#include "graphs_view.h"

#include <imgui.h>
#include <implot.h>

namespace fft_visualizer::view {

constexpr size_t kMaxViewSize = 1 << 14;

GraphsView::GraphsView(WorkerModel& worker_model, std::string window_name)
    : worker_model_{worker_model}, window_name_{window_name} {}

auto GraphsView::GetWindowName() -> const std::string& { return window_name_; }

void GraphsView::Render() {
    if (ImGui::Begin(window_name_.c_str())) {
        CenterText("Graphs");
        ImGui::Separator();

        RenderSignalsGraph();
        RenderHarmonicsGraph();

        ImGui::End();
    }

    run_version_ = worker_model_.GetRunVersion();
}

void GraphsView::RenderSignalsGraph() {
    decltype(auto) initial_signal = worker_model_.GetInitialSignal();
    decltype(auto) recovered_signal = worker_model_.GetRecoveredSignal();

    if (const auto run_version = worker_model_.GetRunVersion(); run_version_ != run_version) {
        auto min_value = std::numeric_limits<float>::max();
        auto max_value = std::numeric_limits<float>::min();

        if (initial_signal) {
            min_value = std::min(min_value, *std::min_element(std::begin(*initial_signal),
                                                              std::end(*initial_signal)));
            max_value = std::max(max_value, *std::max_element(std::begin(*initial_signal),
                                                              std::end(*initial_signal)));
        }

        if (recovered_signal) {
            min_value = std::min(min_value, *std::min_element(std::begin(*recovered_signal),
                                                              std::end(*recovered_signal)));
            max_value = std::max(max_value, *std::max_element(std::begin(*recovered_signal),
                                                              std::end(*recovered_signal)));
        }

        ImPlot::SetNextPlotLimits(0, initial_signal->size(), min_value, max_value,
                                  ImGuiCond_Always);
    }

    const auto plot_height = ImGui::GetContentRegionAvail().y * 0.5f;
    if (ImPlot::BeginPlot("Signals", nullptr, nullptr, {-1, plot_height})) {
        if (initial_signal) {
            size_t size = CalculatePlotSize(initial_signal->size());
            size_t stride = CalculatePlotStride(initial_signal->size());
            ImPlot::PlotLine("Initial", initial_signal->data(), size, stride, 0, 0,
                             stride * sizeof(float));
        }
        if (recovered_signal) {
            size_t size = CalculatePlotSize(recovered_signal->size());
            size_t stride = CalculatePlotStride(recovered_signal->size());
            ImPlot::PlotLine("Recovered", recovered_signal->data(), size, stride, 0, 0,
                             stride * sizeof(float));
        }
        ImPlot::EndPlot();
    }
}

void GraphsView::RenderHarmonicsGraph() {
    decltype(auto) harmonic_amplitudes = worker_model_.GetHarmonicAmplitudes();
    decltype(auto) harmonic_phases = worker_model_.GetHarmonicPhases();

    if (const auto run_version = worker_model_.GetRunVersion(); run_version_ != run_version) {
        auto min_value = std::numeric_limits<float>::max();
        auto max_value = std::numeric_limits<float>::min();
        size_t max_size = 0;

        if (harmonic_amplitudes) {
            min_value = std::min(min_value, *std::min_element(std::begin(*harmonic_amplitudes),
                                                              std::end(*harmonic_amplitudes)));
            max_value = std::max(max_value, *std::max_element(std::begin(*harmonic_amplitudes),
                                                              std::end(*harmonic_amplitudes)));
            max_size = std::max(max_size, harmonic_amplitudes->size());
        }

        if (harmonic_phases) {
            min_value = std::min(min_value, *std::min_element(std::begin(*harmonic_phases),
                                                              std::end(*harmonic_phases)));
            max_value = std::max(max_value, *std::max_element(std::begin(*harmonic_phases),
                                                              std::end(*harmonic_phases)));
            max_size = std::max(max_size, harmonic_phases->size());
        }

        ImPlot::SetNextPlotLimits(0, max_size, min_value, max_value, ImGuiCond_Always);
    }

    const auto plot_height = ImGui::GetContentRegionAvail().y;
    if (ImPlot::BeginPlot("Harmonics", nullptr, nullptr, {-1, plot_height})) {
        if (harmonic_amplitudes) {
            size_t size = CalculatePlotSize(harmonic_amplitudes->size());
            size_t stride = CalculatePlotStride(harmonic_amplitudes->size());
            ImPlot::PlotStems("Amplitudes", harmonic_amplitudes->data(), size, 0, stride, 0, 0,
                              stride * sizeof(float));
        }
        if (harmonic_phases) {
            size_t size = CalculatePlotSize(harmonic_phases->size());
            size_t stride = CalculatePlotStride(harmonic_phases->size());
            ImPlot::PlotStems("Phases", harmonic_phases->data(), size, 0, stride, 0, 0,
                              stride * sizeof(float));
        }
        ImPlot::EndPlot();
    }
}
auto GraphsView::CalculatePlotStride(size_t count) -> size_t {
    const auto plot_size = CalculatePlotSize(count);
    return count / plot_size;
}

auto GraphsView::CalculatePlotSize(size_t count) -> size_t { return std::min(kMaxViewSize, count); }

}  // namespace fft_visualizer::view
