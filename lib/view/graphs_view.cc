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
        RenderHarmonicsGraph();

        ImGui::End();
    }

    run_version_ = worker_model_.GetRunVersion();
}

void GraphsView::RenderSignalsGraph() {
    decltype(auto) initial_signal = worker_model_.GetInitialSignal();
    decltype(auto) recovered_signal = worker_model_.GetRecoveredSignal();

    if (const auto run_version = worker_model_.GetRunVersion(); run_version_ != run_version) {
        const auto min_initial_value =
            *std::min_element(std::begin(*initial_signal), std::end(*initial_signal));
        const auto max_initial_value =
            *std::max_element(std::begin(*initial_signal), std::end(*initial_signal));

        const auto min_recovered_value =
            *std::min_element(std::begin(*recovered_signal), std::end(*recovered_signal));
        const auto max_recovered_value =
            *std::max_element(std::begin(*recovered_signal), std::end(*recovered_signal));

        ImPlot::SetNextPlotLimits(
            0, initial_signal->size(), std::min(min_initial_value, min_recovered_value),
            std::max(max_initial_value, max_recovered_value), ImGuiCond_Always);
    }

    const auto plot_height = ImGui::GetContentRegionAvail().y * 0.5f;
    if (ImPlot::BeginPlot("Signals", nullptr, nullptr, {-1, plot_height})) {
        if (initial_signal) {
            ImPlot::PlotLine("Initial", initial_signal->data(), initial_signal->size());
        }
        if (recovered_signal) {
            ImPlot::PlotLine("Recovered", recovered_signal->data(), recovered_signal->size());
        }
        ImPlot::EndPlot();
    }
}

void GraphsView::RenderHarmonicsGraph() {
    decltype(auto) harmonic_amplitudes = worker_model_.GetHarmonicAmplitudes();
    decltype(auto) harmonic_phases = worker_model_.GetHarmonicPhases();

    if (const auto run_version = worker_model_.GetRunVersion(); run_version_ != run_version) {
        const auto min_amplitude =
            *std::min_element(std::begin(*harmonic_amplitudes), std::end(*harmonic_amplitudes));
        const auto max_amplitude =
            *std::max_element(std::begin(*harmonic_amplitudes), std::end(*harmonic_amplitudes));

        const auto min_phase =
            *std::min_element(std::begin(*harmonic_phases), std::end(*harmonic_phases));
        const auto max_phase =
            *std::max_element(std::begin(*harmonic_phases), std::end(*harmonic_phases));

        ImPlot::SetNextPlotLimits(0, harmonic_amplitudes->size(),
                                  std::min(min_phase, min_amplitude),
                                  std::max(max_phase, max_amplitude), ImGuiCond_Always);
    }

    const auto plot_height = ImGui::GetContentRegionAvail().y;
    if (ImPlot::BeginPlot("Harmonics", nullptr, nullptr, {-1, plot_height})) {
        if (harmonic_amplitudes) {
            ImPlot::PlotStems("Amplitudes", harmonic_amplitudes->data(),
                              harmonic_amplitudes->size());
        }
        if (harmonic_phases) {
            ImPlot::PlotStems("Phases", harmonic_phases->data(), harmonic_phases->size());
        }
        ImPlot::EndPlot();
    }
}

}  // namespace fft_visualizer::view
