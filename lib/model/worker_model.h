#ifndef FFT_VISUALIZER_WORKER_MODEL_H
#define FFT_VISUALIZER_WORKER_MODEL_H

#include <worker/worker.h>

#include <memory>
#include <optional>

#include "opencl_model.h"

namespace fft_visualizer::model {

class FftVisualizerModel;

enum class AlgorithmType {
    kTrivial,
    kFast,
};

class WorkerModel {
  public:
    using Model = FftVisualizerModel;
    using Signal = worker::Worker::Signal;
    using ComplexSignal = worker::Worker::ComplexSignal;

    struct Configuration {
        AlgorithmType algorithm_type{model::AlgorithmType::kTrivial};
        AlgorithmType inverse_algorithm_type{model::AlgorithmType::kTrivial};

        bool erase_phases{false};
        unsigned int preserve_harmonics_from{0};
        unsigned int preserve_harmonics_to{std::numeric_limits<int>::max()};
    };

    explicit WorkerModel(Model& model, OpenClModel& opencl_model);

    auto GetOpenClModel() -> OpenClModel&;
    void InitWorker(std::optional<cl::Device> device);
    void RunWorker(Configuration configuration);

    auto GetRunVersion() -> unsigned int;

    void SetInitialSignal(const Signal& signal);
    auto GetInitialSignal() -> const std::optional<Signal>&;

    auto GetRecoveredSignal() -> const std::optional<Signal>&;

    auto GetHarmonicAmplitudes() -> const std::optional<Signal>&;
    auto GetHarmonicPhases() -> const std::optional<Signal>&;

  private:
    Model& model_;
    OpenClModel& opencl_model_;

    std::optional<Signal> initial_signal_{};
    std::optional<Signal> recovered_signal_{};

    std::optional<ComplexSignal> harmonics_{};
    std::optional<Signal> harmonics_amplitudes_{};
    std::optional<Signal> harmonics_phases_{};

    unsigned int run_version_{0};
    std::unique_ptr<worker::Worker> worker_;

    void TransformHarmonics(ComplexSignal& harmonics, const Configuration& configuration);
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_WORKER_MODEL_H
