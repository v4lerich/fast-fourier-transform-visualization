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

    explicit WorkerModel(Model& model, OpenClModel& opencl_model);

    auto GetOpenClModel() -> OpenClModel&;
    void InitWorker(std::optional<cl::Device> device);

  private:
    Model& model_;
    OpenClModel& opencl_model_;

    std::unique_ptr<worker::Worker> worker_;
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_WORKER_MODEL_H
