#include "fft_visualizer_model.h"

#include <CL/cl.hpp>

namespace fft_visualizer::model {

FftVisualizerModel::FftVisualizerModel()
    : opencl_model_{*this}, worker_model_{*this, opencl_model_} {}

void FftVisualizerModel::SetCurrentError(std::optional<Error> error) { error_ = error; }

auto FftVisualizerModel::GetCurrentError() -> const std::optional<Error>& { return error_; }

auto FftVisualizerModel::GetOpenClModel() -> OpenClModel& { return opencl_model_; }

auto FftVisualizerModel::GetWorkerModel() -> WorkerModel& { return worker_model_; }

}  // namespace fft_visualizer::model
