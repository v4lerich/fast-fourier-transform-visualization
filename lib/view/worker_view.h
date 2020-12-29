#ifndef FFT_VISUALIZER_WORKER_VIEW_H
#define FFT_VISUALIZER_WORKER_VIEW_H

#include <worker_model.h>

#include "view.h"

namespace fft_visualizer::view {

class WorkerView : public View {
  public:
    using Model = model::WorkerModel;
    explicit WorkerView(Model& model, std::string window_name = "worker_view");

    void Render() override;

    auto GetWindowName() -> const std::string&;

  private:
    Model& model_;
    std::string window_name_;
};

}  // namespace fft_visualizer::view

#endif  // FFT_VISUALIZER_WORKER_VIEW_H
