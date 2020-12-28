#ifndef FFT_VISUALIZER_GARMONIC_SIGNAL_GENERATOR_H
#define FFT_VISUALIZER_GARMONIC_SIGNAL_GENERATOR_H

#include "signal_generator.h"

namespace fft_visualizer::model::signal_generator {

class GarmonicSignalGenerator final : public SignalGenerator {
  public:
    struct Parameters {
        float amplitude;
        float phase;
        float garmonic;
    };

    GarmonicSignalGenerator(Parameters parameters);
    auto Generate(unsigned int count) -> Signal const override;

  private:
    Parameters parameters_;
};

}  // namespace fft_visualizer::model::signal_generator

#endif  // FFT_VISUALIZER_GARMONIC_SIGNAL_GENERATOR_H
