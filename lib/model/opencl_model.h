#ifndef FFT_VISUALIZER_OPENCL_MODEL_H
#define FFT_VISUALIZER_OPENCL_MODEL_H

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
    auto GetInfo() -> std::optional<OpenClInfo>;

    void ResetDevice(std::optional<cl::Device> device);
    auto GetDevice() -> const std::optional<cl::Device>&;
    auto GetContext() -> const std::optional<cl::Context>&;

  private:
    Model& model_;

    std::optional<cl::Context> context_;
    std::optional<cl::Device> device_;
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_OPENCL_MODEL_H
