#include "fft_visualizer_view.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <string>

namespace fft_visualizer::view {

const std::string kDockspaceName = "main_dockspace";

FftVisualizerView::FftVisualizerView(FftVisualizerView::Model& model)
    : model_{model},
      opencl_info_view_{model.GetOpenClModel()},
      worker_picker_view_{model.GetWorkerModel()},
      worker_view_{model.GetWorkerModel()},
      graphs_view_{model.GetWorkerModel()} {}

void FftVisualizerView::Render() {
    BeginDockingWindow();
    RenderMenuBar();

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

    ImGui::SetNextWindowClass(&window_class);
    worker_view_.Render();

    ImGui::SetNextWindowClass(&window_class);
    graphs_view_.Render();

    EndDockingWindow();

    opencl_info_view_.Render();
    worker_picker_view_.Render();
    RenderErrorPopup();
}

auto FftVisualizerView::BeginDockingWindow() -> bool {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                    ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{});
    bool is_visible = ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    if (is_visible) {
        InitDockingLayout();
    }
    ImGui::DockSpace(ImGui::GetID(kDockspaceName.c_str()), {0, 0});
    return is_visible;
}

void FftVisualizerView::InitDockingLayout() {
    const ImGuiID dockspace_id = ImGui::GetID(kDockspaceName.c_str());
    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowViewport()->Size);

        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_left_id =
            ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.4, nullptr, &dock_main_id);

        ImGui::DockBuilderDockWindow(worker_view_.GetWindowName().c_str(), dock_left_id);
        ImGui::DockBuilderDockWindow(graphs_view_.GetWindowName().c_str(), dock_main_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }
}

void FftVisualizerView::EndDockingWindow() { ImGui::End(); }

void FftVisualizerView::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                SetWantClose();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Worker")) {
            if (ImGui::MenuItem("Choose device")) {
                worker_picker_view_.Show();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Info")) {
            if (ImGui::MenuItem("OpenCl")) {
                opencl_info_view_.Show();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void FftVisualizerView::RenderErrorPopup() {
    auto error = model_.GetCurrentError();
    std::string window_title = std::string(error ? error->WhatTitle() : "") + "##error";
    if (error) {
        ImGui::OpenPopup(window_title.c_str());
        show_error_ = true;
    }

    const ImVec2 window_size{ImGui::GetIO().DisplaySize.x * 0.7f,
                             ImGui::GetIO().DisplaySize.y * 0.7f};
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal(window_title.c_str(), &show_error_)) {
        if (error) {
            ImGui::Text("%s", error->what());
            ImGui::Separator();
        }
        ImGui::EndPopup();
    }

    if (!show_error_) model_.SetCurrentError(std::nullopt);
}

}  // namespace fft_visualizer::view
