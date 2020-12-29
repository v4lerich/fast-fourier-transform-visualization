//
// Created by User on 12/29/2020.
//

#include "cpu_worker.h"

#include <numbers>

namespace fft_visualizer::model {

auto CpuWorker::DiscreteFourierTransform(const Signal& signal) -> ComplexSignal {
    using namespace std::complex_literals;

    auto n = signal.size();
    auto m = n / 2 + 1;

    ComplexSignal harmonics(m);
    for (unsigned int j = 0; j < m; j++) {
        auto& harmonic = harmonics[j];
        for (unsigned int i = 0; i < n; i++) {
            std::complex<float> phase = float(2 * std::numbers::pi * i * j / n) * 1if;
            harmonic += 2.0f / n * signal[i] * std::exp(phase);
        }
    }
    return harmonics;
}

auto CpuWorker::InverseDiscreteFourierTransform(const ComplexSignal& harmonics) -> Signal {
    auto m = harmonics.size();
    auto n = m * 2;

    Signal signal(n, 0);
    for (unsigned int i = 0; i < n; i++) {
        auto& value = signal[i];

        for (unsigned int j = 0; j < m; j++) {
            const auto& harmonic = harmonics[j];

            float phase = 2 * std::numbers::pi * i * j / n - std::arg(harmonic);
            value += std::norm(harmonic) * std::cos(phase);
        }
    }
    return signal;
}

//auto CpuWorker::FastFourierTransform(const Signal& signal) -> ComplexSignal {}

//auto CpuWorker::InverseFastFourierTransform(const ComplexSignal& signal) -> Signal {}

}  // namespace fft_visualizer::model