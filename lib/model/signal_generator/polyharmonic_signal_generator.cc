#include "polyharmonic_signal_generator.h"

#include <numbers>

namespace fft_visualizer::model::signal_generator {

auto PolyharmonicSignalGenerator::Generate(unsigned int count) -> const SignalGenerator::Signal {
    Signal signal(count);

    for (unsigned int i = 0; i < count; i++) {
        float value = 0;
        for (unsigned int j = 0; j < harmonics_parameters.size(); j++) {
            const auto &parameter = harmonics_parameters[j];

            float phase = 2 * std::numbers::pi * (j + 1) * i / count - parameter.phase;
            value += parameter.amplitude * std::cos(phase);
        }
        signal[i] = value;
    }

    return signal;
}

}  // namespace fft_visualizer::model::signal_generator
