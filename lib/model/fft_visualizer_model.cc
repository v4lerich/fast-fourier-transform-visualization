#include "fft_visualizer_model.h"

#include <CL/cl.hpp>

namespace fft_visualizer::model {

FftVisualizerModel::FftVisualizerModel() : opencl_model_{*this} {}

void FftVisualizerModel::SetCurrentError(std::optional<Error> error) { error_ = error; }

auto FftVisualizerModel::GetOpenClModel() -> OpenClModel& { return opencl_model_; }

}  // namespace fft_visualizer::model
