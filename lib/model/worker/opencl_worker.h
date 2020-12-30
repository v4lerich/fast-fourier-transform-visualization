#ifndef FFT_VISUALIZER_OPENCL_WORKER_H
#define FFT_VISUALIZER_OPENCL_WORKER_H

#include <filesystem>

#include "opencl.h"
#include "worker.h"

namespace fft_visualizer::model::worker {

class OpenClWorker : public Worker {
  public:
    explicit OpenClWorker(const cl::Context& context);

    auto DiscreteFourierTransform(const Signal& signal) -> ComplexSignal override;
    auto InverseDiscreteFourierTransform(const ComplexSignal& garmonics) -> Signal override;

    auto FastFourierTransform(const Signal& signal) -> ComplexSignal override;
    auto InverseFastFourierTransform(const ComplexSignal& garmonics) -> Signal override;

  private:
    typedef cl::make_kernel<cl_uint, cl::Buffer&, cl_uint, cl::Buffer&> FourierKernel;
    typedef cl::make_kernel<cl_uint, cl::Buffer&, cl::Buffer&, cl_uint, cl_uint> FftStepKernel;
    typedef cl::make_kernel<cl_uint, cl::Buffer&> FftDivideStepKernel;
    typedef cl::make_kernel<cl_uint, cl::Buffer&> FftShuffleKernel;

    void GenericFastFourierTransform(ComplexSignal& harmonics, bool invert);
    auto CalculateEnqueueArgs(cl::CommandQueue& queue, size_t n) -> cl::EnqueueArgs const;

    template <typename T>
    auto LinkKernel(const cl::Program& program, const std::string& kernel_name) -> T;
    auto ImportProgram(const cl::Context& context, const std::filesystem::path& path)
        -> cl::Program;
    auto BuildProgram(const cl::Context& context, const cl::Program::Sources& program_sources)
        -> cl::Program;

    const cl::Context& context_;

    cl::Program dft_program_;
    FourierKernel dft_kernel_;
    FourierKernel inverse_dft_kernel_;

    cl::Program fft_program_;
    FftStepKernel fft_step_kernel_;
    FftDivideStepKernel fft_divide_step_kernel_;
    FftShuffleKernel fft_shuffle_kernel_;
};

}  // namespace fft_visualizer::model::worker

#endif  // FFT_VISUALIZER_OPENCL_WORKER_H
