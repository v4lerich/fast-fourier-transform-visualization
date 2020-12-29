#ifndef FFT_VISUALIZER_SIGNAL_GENERATOR_H
#define FFT_VISUALIZER_SIGNAL_GENERATOR_H

#include <complex>
#include <vector>

namespace fft_visualizer::model::signal_generator {

class SignalGenerator {
  public:
    using Signal = std::vector<float>;

    virtual auto Generate(unsigned int count) -> Signal const = 0;
};

}  // namespace fft_visualizer::model::signal_generators

#endif  // FFT_VISUALIZER_SIGNAL_GENERATOR_H
