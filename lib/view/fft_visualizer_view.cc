#include "fft_visualizer_view.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <string>

namespace fft_visualizer::view {

const std::string kDockspaceName = "main_dockspace";

FftVisualizerView::FftVisualizerView(FftVisualizerView::Model& model)
    : opencl_info_view_{model.GetOpenClModel()}, worker_picker_view_{model.GetWorkerModel()} {}

void FftVisualizerView::Render() {
    if (BeginDockingWindow()) {
        RenderMenuBar();

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

        EndDockingWindow();
    }

    opencl_info_view_.Render();
    worker_picker_view_.Render();
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
        ImGui::DockSpace(ImGui::GetID(kDockspaceName.c_str()), {0, 0});
    }
    return is_visible;
}

void FftVisualizerView::InitDockingLayout() {
    const ImGuiID dockspace_id = ImGui::GetID(kDockspaceName.c_str());
    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowViewport()->Size);

        ImGuiID dock_main_id = dockspace_id;

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

}  // namespace fft_visualizer::view
