#include "harmonic_signal_generator.h"

#include <numbers>

namespace fft_visualizer::model::signal_generator {

HarmonicSignalGenerator::HarmonicSignalGenerator(Parameters parameters) : parameters_{parameters} {}

auto HarmonicSignalGenerator::Generate(unsigned int count) -> const Signal {
    Signal signal(count);

    for (unsigned int i = 0; i < count; i++) {
        float phase = 2 * std::numbers::pi * parameters_.harmonic * i / count + parameters_.phase;
        signal[i] = parameters_.amplitude * std::cos(phase);
    }

    return signal;
}

}  // namespace fft_visualizer::model::signal_generator
