#ifndef FFT_VISUALIZER_FFT_VISUALIZER_MODEL_H
#define FFT_VISUALIZER_FFT_VISUALIZER_MODEL_H

#include <optional>

#include "error.h"
#include "open_cl_model.h"

namespace fft_visualizer::model {

class FftVisualizerModel final  {
  public:
    FftVisualizerModel();

    auto GetOpenClModel() -> OpenClModel&;
    void SetCurrentError(std::optional<Error> error);

  private:
    std::optional<Error> error_;
    OpenClModel opencl_model_;
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_FFT_VISUALIZER_MODEL_H
