#include "worker_picker_view.h"

#include <imgui.h>

#include <string>

namespace fft_visualizer::view {

const std::string kWorkerPopupName = "Choose worker";
const std::string kDefaultPlatformName = "None (CPU)";

WorkerPickerView::WorkerPickerView(Model& model) : model_{model} {}

void WorkerPickerView::Show() { show_ = true; }

void WorkerPickerView::Render() {
    if (show_) ImGui::OpenPopup(kWorkerPopupName.c_str());

    const ImVec2 window_size{ImGui::GetIO().DisplaySize.x * 0.7f,
                             ImGui::GetIO().DisplaySize.y * 0.7f};

    ImGui::SetNextWindowSize(window_size, ImGuiCond_Appearing);
    if (ImGui::BeginPopupModal(kWorkerPopupName.c_str(), &show_)) {
        if (const auto opencl_info = model_.GetOpenClModel().GetInfo()) {
            RenderPlatformCombo(*opencl_info);
            RenderDeviceCombo(*opencl_info);

            if (ImGui::Button("OK", ImVec2(120, 0))) {
                model_.InitWorker(current_device_);
                ImGui::CloseCurrentPopup();
                show_ = false;
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
                show_ = false;
            }
        }

        ImGui::EndPopup();
    } else {
        current_device_ = model_.GetOpenClModel().GetDevice();
        current_platform_ =
            current_device_
                ? std::make_optional<cl::Platform>(current_device_->getInfo<CL_DEVICE_PLATFORM>())
                : std::nullopt;
    }
}

void WorkerPickerView::RenderDeviceCombo(const model::OpenClInfo& opencl_info) {
    if (current_platform_) {
        const auto selected_device_name = current_device_->getInfo<CL_DEVICE_NAME>();

        ImGui::Text("Choose OpenCL device:");
        ImGui::Indent();
        if (ImGui::BeginCombo("OpenCL Device", selected_device_name.c_str())) {
            for (const auto& platform_info : opencl_info.platforms) {
                if ((*current_platform_)() != platform_info.platform()) continue;

                for (const auto& device_info : platform_info.devices) {
                    bool is_selected =
                        current_device_ && (*current_device_)() == device_info.device();
                    if (ImGui::Selectable(device_info.device.getInfo<CL_DEVICE_NAME>().c_str(),
                                          is_selected))
                        current_device_ = device_info.device;

                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
        ImGui::Unindent();
    }
}

void WorkerPickerView::RenderPlatformCombo(const model::OpenClInfo& opencl_info) {
    const auto selected_platform_name =
        current_platform_ ? current_platform_->getInfo<CL_PLATFORM_NAME>() : kDefaultPlatformName;
    ImGui::Text("Choose OpenCL platform:");
    ImGui::Indent();
    if (ImGui::BeginCombo("OpenCL Platform", selected_platform_name.c_str())) {
        for (const auto& platform_info : opencl_info.platforms) {
            bool is_selected =
                current_platform_ && (*current_platform_)() == platform_info.platform();
            if (ImGui::Selectable(platform_info.platform.getInfo<CL_PLATFORM_NAME>().c_str(),
                                  is_selected)) {
                current_platform_ = platform_info.platform;
                if (platform_info.devices.empty())
                    current_device_ = std::nullopt;
                else
                    current_device_ = platform_info.devices.front().device;
            }

            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        {
            bool is_selected = !current_platform_;
            if (ImGui::Selectable(kDefaultPlatformName.c_str(), is_selected)) {
                current_platform_ = std::nullopt;
                current_device_ = std::nullopt;
            }

            if (is_selected) ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }
    ImGui::Unindent();
}

}  // namespace fft_visualizer::view
