#ifndef FFT_VISUALIZER_GRAPHS_VIEW_H
#define FFT_VISUALIZER_GRAPHS_VIEW_H

#include <worker_model.h>

#include "view.h"

namespace fft_visualizer::view {

class GraphsView final : public View {
  public:
    using WorkerModel = model::WorkerModel;
    explicit GraphsView(WorkerModel& worker_model, std::string window_name = "graphs_view");

    void Render() override;
    auto GetWindowName() -> const std::string&;

  private:
    void RenderSignalsGraph();
    void RenderHarmonicsGraph();
    auto CalculatePlotStride(size_t count) -> size_t;
    auto CalculatePlotSize(size_t count) -> size_t;

    WorkerModel& worker_model_;
    std::string window_name_;

    unsigned int run_version_{0};
};

}  // namespace fft_visualizer::view

#endif  // FFT_VISUALIZER_GRAPHS_VIEW_H
