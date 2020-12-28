#ifndef FFT_VISUALIZER_FFT_VISUALIZER_VIEW_H
#define FFT_VISUALIZER_FFT_VISUALIZER_VIEW_H

#include <fft_visualizer_model.h>

#include "view.h"
#include "open_cl_info_view.h"

namespace fft_visualizer::view {

class FftVisualizerView final : public View {
  public:
    using Model = model::FftVisualizerModel;

    explicit FftVisualizerView(Model& model);
    void Render() override;

  private:
    bool BeginDockingWindow();
    void EndDockingWindow();
    void InitDockingLayout();
    void RenderMenuBar();

    OpenClInfoView opencl_info_view_;
};

}  // namespace fft_visualizer::view

#endif  // FFT_VISUALIZER_FFT_VISUALIZER_VIEW_H
