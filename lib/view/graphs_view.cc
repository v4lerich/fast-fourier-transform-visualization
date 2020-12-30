#include "graphs_view.h"

#include <imgui.h>
#include <implot.h>

namespace fft_visualizer::view {

GraphsView::GraphsView(WorkerModel& worker_model, std::string window_name)
    : worker_model_{worker_model}, window_name_{window_name} {}

auto GraphsView::GetWindowName() -> const std::string& { return window_name_; }

void GraphsView::Render() {
    if (ImGui::Begin(window_name_.c_str())) {
        CenterText("Graphs");
        ImGui::Separator();

        RenderSignalsGraph();

        ImGui::End();
    }
}

void GraphsView::RenderSignalsGraph() {
    if (auto initial_signal_version = worker_model_.GetInitialSignalVersion();
        initial_signal_version_ != initial_signal_version) {
        initial_signal_version_ = initial_signal_version;

        decltype(auto) initial_signal = worker_model_.GetInitialSignal();
        auto min_value = *std::min_element(std::begin(*initial_signal), std::end(*initial_signal));
        auto max_value = *std::max_element(std::begin(*initial_signal), std::end(*initial_signal));
        ImPlot::SetNextPlotLimits(0, initial_signal->size(), min_value, max_value,
                                  ImGuiCond_Always);
    }

    if (ImPlot::BeginPlot("Signals")) {
        if (decltype(auto) initial_signal = worker_model_.GetInitialSignal()) {
            ImPlot::PlotLine("Initial", initial_signal->data(), initial_signal->size());
        }
        ImPlot::EndPlot();
    }
}

}  // namespace fft_visualizer::view
