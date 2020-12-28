#ifndef FFT_VISUALIZER_POLYGARMONIC_SIGNAL_GENERATOR_H
#define FFT_VISUALIZER_POLYGARMONIC_SIGNAL_GENERATOR_H

#include <vector>

#include "signal_generator.h"

namespace fft_visualizer::model::signal_generator {

class PolygarmonicSignalGenerator final : public SignalGenerator {
  public:
    struct GarmonicParameters {
        float amplitude;
        float phase;
    };

    template <typename IteratorType>
    requires std::same_as<PolygarmonicSignalGenerator::GarmonicParameters,
                          typename std::iterator_traits<IteratorType>::value_type>
    PolygarmonicSignalGenerator(IteratorType from, IteratorType to) {
        for (IteratorType it = from; it != to; it++) {
            garmonics_parameters.push_back(*it);
        }
    }

    auto Generate(unsigned int count) -> Signal const override;

  private:
    std::vector<GarmonicParameters> garmonics_parameters;
};

}  // namespace fft_visualizer::model::signal_generator

#endif  // FFT_VISUALIZER_POLYGARMONIC_SIGNAL_GENERATOR_H
