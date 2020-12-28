#ifndef FFT_VISUALIZER_OPEN_CL_MODEL_H
#define FFT_VISUALIZER_OPEN_CL_MODEL_H

#include <optional>
#include <vector>

#include "opencl.h"

namespace fft_visualizer::model {


struct OpenClDeviceInfo {
    cl::Device device;
};

struct OpenClPlatformInfo {
    cl::Platform platform;
    std::vector<OpenClDeviceInfo> devices;
};

struct OpenClInfo {
    std::vector<OpenClPlatformInfo> platforms;
};

class FftVisualizerModel;

class OpenClModel final {
  public:
    using Model = FftVisualizerModel;

    OpenClModel(Model& model);
    auto GetOpenClInfo() -> std::optional<OpenClInfo>;

  private:
    Model& model_;
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_OPEN_CL_MODEL_H
