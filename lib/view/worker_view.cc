#include "worker_view.h"

#include <imgui.h>

#include <utility>

namespace fft_visualizer::view {

WorkerView::WorkerView(Model& model, std::string window_name)
    : model_{model}, window_name_{std::move(window_name)} {}

void WorkerView::Render() {
    if (ImGui::Begin(window_name_.c_str())) {
        const std::string header = "Parameters";
        const float font_size = ImGui::GetFontSize() * header.size() / 2;
        ImGui::SameLine(ImGui::GetWindowSize().x / 2 - (font_size / 2));
        ImGui::Text("%s", header.c_str());
        ImGui::Separator();

        ImGui::End();
    }
}

auto WorkerView::GetWindowName() -> const std::string& { return window_name_; }

}  // namespace fft_visualizer::view
