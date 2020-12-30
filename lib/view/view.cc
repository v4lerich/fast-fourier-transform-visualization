#include "view.h"

#include <imgui.h>

#include <map>
#include <numbers>
#include <random>
#include <utility>

#include "worker_view.h"

void fft_visualizer::view::View::CenterText(const std::string& text) {
    const float font_size = ImGui::GetFontSize() * text.size() / 2;
    ImGui::SameLine(ImGui::GetWindowSize().x / 2 - (font_size / 2));
    ImGui::Text("%s", text.c_str());
}
namespace fft_visualizer::view {

auto View::WantClose() -> bool { return want_close_; }

void View::SetWantClose() { want_close_ = true; }

}  // namespace fft_visualizer::view
