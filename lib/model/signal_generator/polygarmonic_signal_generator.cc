#include "polygarmonic_signal_generator.h"

#include <numbers>

namespace fft_visualizer::model::signal_generator {

auto PolygarmonicSignalGenerator::Generate(unsigned int count) -> const SignalGenerator::Signal {
    Signal signal(count);

    for (unsigned int i = 0; i < count; i++) {
        float value = 0;
        for (unsigned int j = 1; j <= garmonics_parameters.size(); j++) {
            const auto &parameter = garmonics_parameters[j];

            float phase = 2 * std::numbers::pi * j * i / count - parameter.phase;
            value += parameter.amplitude * std::cos(phase);
        }
        signal[i] = value;
    }

    return signal;
}

}  // namespace fft_visualizer::model::signal_generator
