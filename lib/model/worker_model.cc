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

auto WorkerModel::GetRunVersion() -> unsigned int { return run_version_; }

void WorkerModel::RunWorker(AlgorithmType algorithm_type, AlgorithmType inverse_algorithm_type) {
    if (initial_signal_) {
        if (algorithm_type == AlgorithmType::kTrivial) {
            harmonics_ = worker_->DiscreteFourierTransform(*initial_signal_);
        } else if (algorithm_type == AlgorithmType::kFast) {
            harmonics_ = worker_->FastFourierTransform(*initial_signal_);
        }

        if (inverse_algorithm_type == AlgorithmType::kTrivial) {
            recovered_signal_ = worker_->InverseDiscreteFourierTransform(*harmonics_);
        } else if (inverse_algorithm_type == AlgorithmType::kFast) {
            recovered_signal_ = worker_->InverseFastFourierTransform(*harmonics_);
        }

        harmonics_amplitudes_ = Signal(harmonics_->size());
        harmonics_phases_ = Signal(harmonics_->size());
        for (size_t i = 0; i < harmonics_->size(); i++) {
            (*harmonics_amplitudes_)[i] = std::abs((*harmonics_)[i]);
            (*harmonics_phases_)[i] = std::arg((*harmonics_)[i]);
        }
    }
}

void WorkerModel::SetInitialSignal(const Signal& signal) {
    run_version_++;
    initial_signal_ = signal;
}

auto WorkerModel::GetInitialSignal() -> const std::optional<Signal>& { return initial_signal_; }

auto WorkerModel::GetHarmonicPhases() -> const std::optional<Signal>& { return harmonics_phases_; }

auto WorkerModel::GetHarmonicAmplitudes() -> const std::optional<Signal>& {
    return harmonics_amplitudes_;
}

auto WorkerModel::GetRecoveredSignal() -> const std::optional<Signal>& { return recovered_signal_; }

}  // namespace fft_visualizer::model
