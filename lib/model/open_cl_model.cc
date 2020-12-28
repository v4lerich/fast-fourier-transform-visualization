#include "open_cl_model.h"

#include <functional>

#include "error.h"
#include "fft_visualizer_model.h"

namespace fft_visualizer::model {

OpenClModel::OpenClModel(OpenClModel::Model& model) : model_{model} {}

auto OpenClModel::GetOpenClInfo() -> std::optional<OpenClInfo> {
    try {
        cl_uint error = CL_SUCCESS;
        std::vector<cl::Platform> platforms;
        OpenClInfo info;

        error = cl::Platform::get(&platforms);
        info.platforms.resize(platforms.size());

        for (size_t i = 0; i < platforms.size(); i++) {
            std::vector<cl::Device> devices;

            const auto& platform = platforms[i];
            auto& platform_info = info.platforms[i];

            platform_info.platform = platform;

            error = platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
            platform_info.devices.resize(devices.size());

            for (size_t j = 0; j < devices.size(); j++) {
                const auto& device = devices[j];
                auto& device_info = platform_info.devices[j];

                device_info.device = device;
            }
        }
        return info;
    } catch (const model::Error& error) {
        model_.SetCurrentError(error);
        return std::nullopt;
    }
}

}  // namespace fft_visualizer::model