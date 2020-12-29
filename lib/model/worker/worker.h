#ifndef FFT_VISUALIZER_WORKER_H
#define FFT_VISUALIZER_WORKER_H

#include <complex>
#include <vector>

namespace fft_visualizer::model::worker {

class Worker {
  public:
    using Signal = std::vector<float>;
    using ComplexSignal = std::vector<std::complex<float>>;

    virtual auto DiscreteFourierTransform(const Signal& signal) -> ComplexSignal = 0;
    virtual auto InverseDiscreteFourierTransform(const ComplexSignal& garmonics) -> Signal = 0;
    virtual auto FastFourierTransform(const Signal& signal) -> ComplexSignal = 0;
    virtual auto InverseFastFourierTransform(const ComplexSignal& garmonics) -> Signal = 0;
};

}  // namespace fft_visualizer::model::worker

#endif  // FFT_VISUALIZER_WORKER_H
