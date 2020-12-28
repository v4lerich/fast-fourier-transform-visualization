//
// Created by User on 12/29/2020.
//

#include "cpu_worker.h"

#include <numbers>

namespace fft_visualizer::model {

auto CpuWorker::DiscreteFourierTransform(const Signal& signal) -> ComplexSignal {
    auto n = signal.size();
    auto m = n / 2 + 1;

    ComplexSignal garmonics(m);
    for (unsigned int j = 0; j < m; j++) {
        auto& garmonic = garmonics[j];
        for (unsigned int i = 0; i < n; i++) {
            std::complex<float> phase = 2 * std::numbers::pi * i * j / n;
            garmonic += 2.0f / n * signal[i] * std::exp(phase);
        }
    }
    return garmonics;
}

auto CpuWorker::InverseDiscreteFourierTransform(const ComplexSignal& garmonics) -> Signal {
    auto m = garmonics.size();
    auto n = m * 2;

    Signal signal(n, 0);
    for (unsigned int i = 0; i < n; i++) {
        auto& value = signal[i];

        for (unsigned int j = 0; j < m; j++) {
            const auto& garmonic = garmonics[j];

            float phase = 2 * std::numbers::pi * i * j / n - std::arg(garmonic);
            value += n * std::norm(garmonic) * std::cos(phase);
        }
    }
    return signal;
}

auto CpuWorker::FastFourierTransform(const Signal& signal) -> ComplexSignal {}

auto CpuWorker::InverseFastFourierTransform(const ComplexSignal& signal) -> Signal {}

}  // namespace fft_visualizer::model