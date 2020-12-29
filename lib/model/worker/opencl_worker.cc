#include "opencl_worker.h"

#include <fstream>

#include "error.h"

namespace fft_visualizer::model::worker {

static const std::filesystem::path kDftProgramPath = "./res/kernels/dft_kernels.cl";
static const std::filesystem::path kFftProgramPath = "./res/kernels/fft_kernels.cl";

OpenClWorker::OpenClWorker(const cl::Context& context)
    : context_{context},
      dft_program_{ImportProgram(context, kDftProgramPath)},
      dft_kernel_{LinkKernel(dft_program_, "dft_kernel")},
      inverse_dft_kernel_{LinkKernel(dft_program_, "inverse_dft_kernel")},

      fft_program_{ImportProgram(context, kFftProgramPath)},
      fft_kernel_{LinkKernel(fft_program_, "fft_kernel")},
      inverse_fft_kernel_{LinkKernel(fft_program_, "inverse_fft_kernel")} {}

auto OpenClWorker::ImportProgram(const cl::Context& context, const std::filesystem::path& path)
    -> cl::Program {
    std::ifstream ifstream{path, std::ios::in};
    if (ifstream.fail()) throw Error{"Kernel source file is not found in: " + path.string()};

    std::string source{std::istreambuf_iterator<char>{ifstream}, std::istreambuf_iterator<char>()};

    cl::Program::Sources program_sources{{source.c_str(), source.size()}};
    return BuildProgram(context, program_sources);
}

auto OpenClWorker::BuildProgram(const cl::Context& context,
                                const cl::Program::Sources& program_sources) -> cl::Program {
    cl::Program program{context, program_sources};
    try {
        program.build();
        return program;
    } catch (cl::Error& error) {
        std::ostringstream ss;
        ss << "Program build error: " << error.what() << std::endl;
        ss << "Program build log: " << std::endl;
        for (auto& device : context.getInfo<CL_CONTEXT_DEVICES>()) {
            cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
            if (status != CL_BUILD_ERROR) continue;

            ss << "\t" << device.getInfo<CL_DEVICE_NAME>() << ": "
               << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        }

        throw Error{ss.str(), "OpenCL program build error"};
    }
}

auto OpenClWorker::LinkKernel(const cl::Program& program, const std::string& kernel_name)
    -> FourierKernel {
    cl_int error = CL_SUCCESS;
    cl::Kernel kernel{program, kernel_name.c_str(), &error};
    if (error != CL_SUCCESS) {
        throw Error{"Undefined reference to kernel: " + kernel_name, "OpenCL compilation error"};
    }
    return {kernel};
}

auto OpenClWorker::DiscreteFourierTransform(const Signal& signal) -> ComplexSignal {
    cl_int error = CL_SUCCESS;
    cl::Event event;
    cl::CommandQueue queue{context_};

    // Create buffers
    unsigned int n = signal.size();
    cl::Buffer signal_buffer{context_, CL_MEM_READ_ONLY, n * sizeof(cl_float), nullptr, &error};

    unsigned int m = n;  // TODO: fix M size to half
    cl::Buffer harmonics_buffer{context_, CL_MEM_WRITE_ONLY, m * sizeof(cl_float2), nullptr,
                                &error};

    queue.enqueueWriteBuffer(signal_buffer, CL_TRUE, 0, n * sizeof(cl_float), signal.data(),
                             nullptr, &event);
    queue.finish();

    // Init worker topology
    cl::NDRange local_size{16};
    size_t local_groups = std::ceil(float(n) / local_size[0]);
    cl::NDRange global_size{local_groups * local_size[0]};
    cl::EnqueueArgs enqueue_args{queue, event, global_size, local_size};

    // Invoke kernel
    dft_kernel_(enqueue_args, n, signal_buffer, m, harmonics_buffer);
    queue.finish();

    // Read result
    std::vector<cl_float2> harmonic_values(m);
    queue.enqueueReadBuffer(harmonics_buffer, CL_TRUE, 0, m * sizeof(cl_float2),
                            harmonic_values.data(), nullptr, &event);

    ComplexSignal harmonics(m);
    for (unsigned int i = 0; i < m; i++) {
        harmonics[i] = {harmonic_values[i].s[0], harmonic_values[i].s[1]};
    }
    return harmonics;
}

auto OpenClWorker::InverseDiscreteFourierTransform(const Worker::ComplexSignal& harmonics)
    -> Worker::Signal {
    cl_int error = CL_SUCCESS;
    cl::Event event;
    cl::CommandQueue queue{context_};

    // Create buffers
    unsigned int m = harmonics.size();
    cl::Buffer harmonics_buffer{context_, CL_MEM_READ_ONLY, m * sizeof(cl_float2), nullptr, &error};

    unsigned int n = m;  // TODO: fix M size to half
    cl::Buffer signal_buffer{context_, CL_MEM_WRITE_ONLY, n * sizeof(cl_float), nullptr, &error};

    std::vector<cl_float2> harmonic_values(m);
    for (unsigned int i = 0; i < m; i++) {
        harmonic_values[i] = {harmonics[i].real(), harmonics[i].imag()};
    }
    queue.enqueueWriteBuffer(harmonics_buffer, CL_TRUE, 0, m * sizeof(cl_float2),
                             harmonic_values.data(), nullptr, &event);
    queue.finish();

    // Init worker topology
    cl::NDRange local_size{16};
    size_t local_groups = std::ceil(float(n) / local_size[0]);
    cl::NDRange global_size{local_groups * local_size[0]};
    cl::EnqueueArgs enqueue_args{queue, event, global_size, local_size};

    // Invoke kernel
    inverse_dft_kernel_(enqueue_args, m, harmonics_buffer, n, signal_buffer);
    queue.finish();

    // Read result
    Signal signal(n);
    queue.enqueueReadBuffer(harmonics_buffer, CL_TRUE, 0, n * sizeof(cl_float), signal.data(),
                            nullptr, &event);
    return signal;
}

}  // namespace fft_visualizer::model::worker
