#include "application.h"

#include <IconsForkAwesome.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
#include <gl_loader.h>
#include <glad/gl.h>
#include <implot.h>

#include "application_configuration.h"

namespace fft_visualizer {

static const std::vector<ImWchar> kForkAwesomeIconsRanges{ICON_MIN_FK, ICON_MAX_FK, 0};

Application::Application(int /*argc*/, char** /*argv*/) {}

auto Application::Init() -> Application::ReturnCode {
    ReturnCode return_code = 0;

    return_code = InitSdl();
    if (return_code != 0) {
        return return_code;
    }

    return_code = CreateMainWindow();
    if (return_code != 0) {
        return return_code;
    }

    return_code = InitImGui();
    return return_code;
}

auto Application::Run() -> Application::ReturnCode {
    SDL_Event sdl_event;
    while (!done_) {
        while (SDL_PollEvent(&sdl_event) > 0) {
            ProcessSdlEvent(sdl_event);
        }

        RenderViews();
        SDL_GL_SwapWindow(main_window_);
    }

    return Finalize();
}

void Application::ProcessSdlEvent(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);

    if (event.type == SDL_QUIT) {
        done_ = true;
    }

    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
        event.window.windowID == SDL_GetWindowID(main_window_)) {
        done_ = true;
    }
}

void Application::RenderViews() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(main_window_);
    ImGui::NewFrame();
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    for (auto& view : views_) {
        view->Render();
        done_ |= view->WantClose();
    }

    ImGui::Render();
    glViewport(0, 0, static_cast<int>(imgui_io_->DisplaySize.x),
               static_cast<int>(imgui_io_->DisplaySize.y));
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

auto Application::InitSdl() -> Application::ReturnCode {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, configuration::kOpenGlVersion.major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, configuration::kOpenGlVersion.minor);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    return 0;
}

Application::ReturnCode Application::CreateMainWindow() {
    const auto window_flags =
        SDL_WindowFlags(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    main_window_ =
        SDL_CreateWindow("Fft visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         configuration::kWindowWidth, configuration::kWindowHeight, window_flags);

    gl_context_ = SDL_GL_CreateContext(main_window_);
    SDL_GL_MakeCurrent(main_window_, gl_context_);
    SDL_GL_SetSwapInterval(configuration::kVsyncValue);

    if (gl_loader::LoadGl()) {
        return -2;
    }
    return 0;
}

auto Application::InitImGui() -> Application::ReturnCode {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    imgui_io_ = &ImGui::GetIO();
    imgui_io_->IniFilename = nullptr;
    imgui_io_->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    imgui_io_->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    imgui_io_->ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

    ImPlot::GetStyle().AntiAliasedLines = true;

    if (configuration::kImGuiTheme == configuration::ApplicationTheme::kDark) {
        ImGui::StyleColorsDark();
    } else if (configuration::kImGuiTheme == configuration::ApplicationTheme::kClassic) {
        ImGui::StyleColorsClassic();
    } else if (configuration::kImGuiTheme == configuration::ApplicationTheme::kLight) {
        ImGui::StyleColorsLight();
    }

    ImGui_ImplSDL2_InitForOpenGL(main_window_, gl_context_);
    ImGui_ImplOpenGL3_Init(configuration::kGlslVersion);

    imgui_io_->Fonts->AddFontFromFileTTF(configuration::kDroidSansFontPath.c_str(), 17.0F);

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    imgui_io_->Fonts->AddFontFromFileTTF(configuration::kForkAwesomeFontPath.c_str(), 17.0F,
                                         &icons_config, kForkAwesomeIconsRanges.data());

    return 0;
}

auto Application::Finalize() -> Application::ReturnCode {
    views_.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(main_window_);
    SDL_Quit();

    return 0;
}

void Application::AddView(std::unique_ptr<view::View> view) { views_.push_back(std::move(view)); }

}  // namespace fft_visualizer
