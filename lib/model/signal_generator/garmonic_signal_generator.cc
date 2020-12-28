#include "garmonic_signal_generator.h"

#include <numbers>

namespace fft_visualizer::model::signal_generator {

GarmonicSignalGenerator::GarmonicSignalGenerator(Parameters parameters) : parameters_{parameters} {}

auto GarmonicSignalGenerator::Generate(unsigned int count) -> const Signal {
    Signal signal(count);

    for (unsigned int i = 0; i < count; i++) {
        float phase = 2 * std::numbers::pi * parameters_.garmonic * i / count + parameters_.phase;
        signal[i] = parameters_.amplitude * std::cos(phase);
    }

    return signal;
}

}  // namespace fft_visualizer::model::signal_generator
