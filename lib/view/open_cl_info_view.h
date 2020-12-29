#ifndef FFT_VISUALIZER_OPEN_CL_INFO_VIEW_H
#define FFT_VISUALIZER_OPEN_CL_INFO_VIEW_H

#include <opencl_model.h>

#include "view.h"

namespace fft_visualizer::view {

class OpenClInfoView final : public View {
  public:
    using Model = model::OpenClModel;
    explicit OpenClInfoView(Model& model);

    void Show();
    void Render() override;

  private:
    void RenderPlatform(const model::OpenClPlatformInfo& platform_info);
    void RenderDevice(const model::OpenClDeviceInfo& device_info);
    auto FormatByteSize(unsigned long long size) -> std::string;

    bool show_{};
    Model& model_;
    std::optional<model::OpenClInfo> opencl_info_{};
};

}  // namespace fft_visualizer::view

#endif  // FFT_VISUALIZER_OPEN_CL_INFO_VIEW_H
