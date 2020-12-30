#ifndef FFT_VISUALIZER_POLYHARMONIC_SIGNAL_GENERATOR_H
#define FFT_VISUALIZER_POLYHARMONIC_SIGNAL_GENERATOR_H

#include <vector>

#include "signal_generator.h"

namespace fft_visualizer::model::signal_generator {

class PolyharmonicSignalGenerator final : public SignalGenerator {
  public:
    struct HarmonicParameters {
        float amplitude = 1.0f;
        float phase;
    };
    using Parameters = std::vector<HarmonicParameters>;

    template <typename IteratorType>
    requires std::same_as<HarmonicParameters,
                          typename std::iterator_traits<IteratorType>::value_type>
    PolyharmonicSignalGenerator(IteratorType from, IteratorType to) {
        for (IteratorType it = from; it != to; it++) {
            harmonics_parameters.push_back(*it);
        }
    }

    auto Generate(unsigned int count) -> Signal const override;

  private:
    Parameters harmonics_parameters;
};

}  // namespace fft_visualizer::model::signal_generator

#endif  // FFT_VISUALIZER_POLYHARMONIC_SIGNAL_GENERATOR_H
