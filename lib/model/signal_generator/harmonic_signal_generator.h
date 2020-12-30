#ifndef FFT_VISUALIZER_HARMONIC_SIGNAL_GENERATOR_H
#define FFT_VISUALIZER_HARMONIC_SIGNAL_GENERATOR_H

#include "signal_generator.h"

namespace fft_visualizer::model::signal_generator {

class HarmonicSignalGenerator final : public SignalGenerator {
  public:
    struct Parameters {
        float amplitude = 1.0f;
        float phase;
        float harmonic = 1.0f;
    };

    HarmonicSignalGenerator(Parameters parameters);
    auto Generate(unsigned int count) -> Signal const override;

  private:
    Parameters parameters_;
};

}  // namespace fft_visualizer::model::signal_generator

#endif  // FFT_VISUALIZER_HARMONIC_SIGNAL_GENERATOR_H
