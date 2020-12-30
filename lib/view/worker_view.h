#ifndef FFT_VISUALIZER_WORKER_VIEW_H
#define FFT_VISUALIZER_WORKER_VIEW_H

#include <signal_generator/harmonic_signal_generator.h>
#include <signal_generator/polyharmonic_signal_generator.h>
#include <worker_model.h>

#include <random>

#include "view.h"

namespace fft_visualizer::view {

enum class SignalType {
    kHarmonic,
    kPolyharmonic,
};

class WorkerView : public View {
  public:
    using Model = model::WorkerModel;

    explicit WorkerView(Model& model, std::string window_name = "worker_view");

    void Render() override;
    auto GetWindowName() -> const std::string&;

  private:
    void RenderNOptionsCombo();
    void RenderSignalTypeCombo();
    void RenderHarmonicSignalParameters();
    void RenderPolyharmonicSignalParameters();
    void RenderAlgorithmTypeCombo(std::string label, model::AlgorithmType& algorithm_type);

    void RandomizeSignalParameters();
    void RunWorker();
    static auto GetNValue(unsigned int index) -> unsigned int;

    Model& model_;
    std::string window_name_;

    model::WorkerModel::Configuration configuration_{};

    unsigned int n_;
    SignalType signal_type_{SignalType::kHarmonic};
    model::signal_generator::HarmonicSignalGenerator::Parameters harmonic_parameters_{};
    model::signal_generator::PolyharmonicSignalGenerator::Parameters polyharmonic_parameters_{1};

    std::random_device random_device_{};
};

}  // namespace fft_visualizer::view

#endif  // FFT_VISUALIZER_WORKER_VIEW_H
