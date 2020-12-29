#ifndef FFT_VISUALIZER_WORKER_PICKER_VIEW_H
#define FFT_VISUALIZER_WORKER_PICKER_VIEW_H

#include <worker_model.h>

#include "view.h"

namespace fft_visualizer::view {

class WorkerPickerView final : public View {
  public:
    using Model = model::WorkerModel;

    explicit WorkerPickerView(Model& model);

    void Show();
    void Render() override;

  private:
    void RenderPlatformCombo(const model::OpenClInfo& opencl_info);
    void RenderDeviceCombo(const model::OpenClInfo& opencl_info);

    Model& model_;
    bool show_{};

    std::optional<cl::Device> current_device_;
    std::optional<cl::Platform> current_platform_;
};

}  // namespace fft_visualizer::view

#endif  // FFT_VISUALIZER_WORKER_PICKER_VIEW_H
