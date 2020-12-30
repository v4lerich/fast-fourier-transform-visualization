#ifndef FFT_VISUALIZER_WORKER_MODEL_H
#define FFT_VISUALIZER_WORKER_MODEL_H

#include <worker/worker.h>

#include <memory>
#include <optional>

#include "opencl_model.h"

namespace fft_visualizer::model {

class FftVisualizerModel;

class WorkerModel {
  public:
    using Model = FftVisualizerModel;
    using Signal = worker::Worker::Signal;

    explicit WorkerModel(Model& model, OpenClModel& opencl_model);

    auto GetOpenClModel() -> OpenClModel&;
    void InitWorker(std::optional<cl::Device> device);

    void SetInitialSignal(const Signal& signal);
    auto GetInitialSignal() -> const std::optional<Signal>&;
    auto GetInitialSignalVersion() -> unsigned int;

  private:
    Model& model_;
    OpenClModel& opencl_model_;

    std::optional<Signal> initial_signal_{};
    unsigned int initial_signal_version_{0};
    std::unique_ptr<worker::Worker> worker_;
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_WORKER_MODEL_H
