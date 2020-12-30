#include "worker_model.h"

#include <worker/cpu_worker.h>
#include <worker/opencl_worker.h>

#include "error.h"
#include "fft_visualizer_model.h"
#include "opencl_model.h"

namespace fft_visualizer::model {

constexpr float kEpsilon = 5e-4;

WorkerModel::WorkerModel(Model& model, OpenClModel& opencl_model)
    : model_{model}, opencl_model_{opencl_model} {
    worker_ = std::make_unique<worker::CpuWorker>();
}

auto WorkerModel::GetOpenClModel() -> OpenClModel& { return opencl_model_; }

void WorkerModel::InitWorker(std::optional<cl::Device> device) {
    opencl_model_.ResetDevice(device);

    if (decltype(auto) context = opencl_model_.GetContext()) {
        try {
            worker_ = std::make_unique<worker::OpenClWorker>(*context);
        } catch (Error& error) {
            model_.SetCurrentError(error);
            opencl_model_.ResetDevice(std::nullopt);
        }
    } else {
        worker_ = std::make_unique<worker::CpuWorker>();
    }
}

auto WorkerModel::GetRunVersion() -> unsigned int { return run_version_; }

void WorkerModel::RunWorker(AlgorithmType algorithm_type, AlgorithmType inverse_algorithm_type) {
    if (initial_signal_) {
        try {
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
                const auto amplitude = std::abs((*harmonics_)[i]);

                (*harmonics_amplitudes_)[i] = std::abs(amplitude) >= kEpsilon ? amplitude : 0;
                (*harmonics_phases_)[i] =
                    std::abs(amplitude) >= kEpsilon ? std::arg((*harmonics_)[i]) : 0;
            }
        } catch (Error& error) {
            model_.SetCurrentError(error);
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
