#include "worker_model.h"

#include <worker/cpu_worker.h>
#include <worker/opencl_worker.h>

#include "opencl_model.h"

namespace fft_visualizer::model {

WorkerModel::WorkerModel(Model& model, OpenClModel& opencl_model)
    : model_{model}, opencl_model_{opencl_model} {
    worker_ = std::make_unique<worker::CpuWorker>();
}

auto WorkerModel::GetOpenClModel() -> OpenClModel& { return opencl_model_; }

void WorkerModel::InitWorker(std::optional<cl::Device> device) {
    opencl_model_.ResetDevice(device);

    if (device) {
        worker_ = std::make_unique<worker::CpuWorker>();
    } else {
        worker_ = std::make_unique<worker::OpenClWorker>(*opencl_model_.GetContext());
    }
}

void WorkerModel::SetInitialSignal(const Signal& signal) {
    initial_signal_version_++;
    initial_signal_ = signal;
}

auto WorkerModel::GetInitialSignal() -> const std::optional<Signal>& { return initial_signal_; }

auto WorkerModel::GetInitialSignalVersion() -> unsigned int { return initial_signal_version_; }

}  // namespace fft_visualizer::model
