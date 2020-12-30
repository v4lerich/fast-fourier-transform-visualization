#ifndef FFT_VISUALIZER_FFT_VISUALIZER_MODEL_H
#define FFT_VISUALIZER_FFT_VISUALIZER_MODEL_H

#include <optional>

#include "error.h"
#include "opencl_model.h"
#include "worker_model.h"

namespace fft_visualizer::model {

class FftVisualizerModel final  {
  public:
    FftVisualizerModel();

    auto GetOpenClModel() -> OpenClModel&;
    auto GetWorkerModel() -> WorkerModel&;

    void SetCurrentError(std::optional<Error> error);
    auto GetCurrentError() -> const std::optional<Error>&;

  private:
    std::optional<Error> error_;
    OpenClModel opencl_model_;
    WorkerModel worker_model_;
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_FFT_VISUALIZER_MODEL_H
