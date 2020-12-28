#include "open_cl_info_view.h"

#include <error.h>
#include <imgui.h>

#include <iomanip>
#include <sstream>

namespace fft_visualizer::view {

OpenClInfoView::OpenClInfoView(OpenClInfoView::Model& model) : model_{model} {}

static const std::string kPopupName = "OpenCL info";

void OpenClInfoView::Show() {
    opencl_info_ = model_.GetOpenClInfo();
    show_ = bool(opencl_info_);
}

void OpenClInfoView::Render() {
    if (show_) {
        ImGui::OpenPopup(kPopupName.c_str());
    }

    const ImVec2 window_size{ImGui::GetIO().DisplaySize.x * 0.7f,
                             ImGui::GetIO().DisplaySize.y * 0.7f};
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal(kPopupName.c_str(), &show_)) {
        ImGui::Text("OpenCL Platforms:");

        ImGuiID id_counter = 0;
        for (const auto& platform : opencl_info_->platforms) {
            ImGui::PushID(id_counter++);
            RenderPlatform(platform);
            ImGui::Separator();
            ImGui::PopID();
        }

        ImGui::EndPopup();
    }
}

void OpenClInfoView::RenderPlatform(const model::OpenClPlatformInfo& platform_info) {
    const auto& platform = platform_info.platform;
    if (ImGui::TreeNode(platform.getInfo<CL_PLATFORM_NAME>().c_str())) {
        ImGui::Indent();
        ImGui::Text("Version:");
        ImGui::BulletText("%s", platform.getInfo<CL_PLATFORM_VERSION>().c_str());

        ImGui::Text("Vendor:");
        ImGui::BulletText("%s", platform.getInfo<CL_PLATFORM_VENDOR>().c_str());

        ImGui::Text("Profile:");
        ImGui::BulletText("%s", platform.getInfo<CL_PLATFORM_PROFILE>().c_str());
        ImGui::Unindent();

        if (ImGui::TreeNode("Extensions:")) {
            ImGui::TextWrapped("%s", platform.getInfo<CL_PLATFORM_EXTENSIONS>().c_str());
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Devices:")) {
            ImGuiID id_counter = 0;
            for (const auto& device : platform_info.devices) {
                ImGui::PushID(id_counter++);
                RenderDevice(device);
                ImGui::Separator();
                ImGui::PopID();
            }
            ImGui::TreePop();
        }

        ImGui::Unindent();
        ImGui::TreePop();
    }
}

void OpenClInfoView::RenderDevice(const model::OpenClDeviceInfo& device_info) {
    const auto& device = device_info.device;
    if (ImGui::TreeNode(device.getInfo<CL_DEVICE_NAME>().c_str())) {
        ImGui::Indent();

        ImGui::Text("Type:");
        ImGui::BulletText("%llu", device.getInfo<CL_DEVICE_TYPE>());

        ImGui::Text("Vendor:");
        ImGui::BulletText("%s", device.getInfo<CL_DEVICE_VENDOR>().c_str());

        ImGui::Text("Max compute units:");
        ImGui::BulletText("%d", device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>());

        ImGui::Text("Max clock frequency:");
        ImGui::BulletText("%d MHz", device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>());

        ImGui::Text("Global memory:");
        ImGui::BulletText("%s",
                          FormatByteSize(device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>()).c_str());

        ImGui::Text("Local memory:");
        ImGui::BulletText("%s", FormatByteSize(device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>()).c_str());
        ImGui::Unindent();

        ImGui::TreePop();
    }
}

auto OpenClInfoView::FormatByteSize(unsigned long long size) -> std::string {
    static const std::string kSuffixes[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};

    double count = size;
    const std::string* suffix_it = kSuffixes;
    while (count >= 1024) {
        count /= 1024;
        suffix_it++;
    }

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << count << " " << *suffix_it;

    return ss.str();
}

}  // namespace fft_visualizer::view
