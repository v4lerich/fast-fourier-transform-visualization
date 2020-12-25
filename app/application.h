#ifndef FFT_VISUALIZER_APPLICATION_H
#define FFT_VISUALIZER_APPLICATION_H

#include <SDL2/SDL.h>
#include <imgui.h>

#include <memory>
#include <vector>
#include <view.h>

namespace fft_visualizer {
class Application {
  public:
    using ReturnCode = int;

    Application(int argc, char** argv);
    auto Init() -> ReturnCode;
    auto Run() -> ReturnCode;

    void AddView(std::unique_ptr<view::View> view);

  private:
    static auto InitSdl() -> ReturnCode;
    auto InitImGui() -> ReturnCode;
    auto CreateMainWindow() -> ReturnCode;
    auto Finalize() -> ReturnCode;

    void ProcessSdlEvent(const SDL_Event& event);
    void RenderViews();

    SDL_Window* main_window_{};
    SDL_GLContext gl_context_{};
    ImGuiIO* imgui_io_{};
    bool done_{};

    std::vector<std::unique_ptr<view::View>> views_{};
};

}  // namespace fft_visualizer

#endif  // FFT_VISUALIZER_APPLICATION_H
