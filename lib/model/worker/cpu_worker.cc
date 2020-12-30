#include "cpu_worker.h"

#include <numbers>
#include <tuple>

namespace fft_visualizer::model::worker {

auto CpuWorker::DiscreteFourierTransform(const Signal& signal) -> ComplexSignal {
    using namespace std::complex_literals;

    const auto n = signal.size();
    const auto m = n;  // TODO: fix M size to half

    ComplexSignal harmonics(m);
    for (unsigned int j = 0; j < m; j++) {
        auto& harmonic = harmonics[j] = 0;
        for (unsigned int i = 0; i < n; i++) {
            std::complex<float> phase = float(2 * std::numbers::pi * i * j / n) * 1if;
            harmonic += 1.0f / n * signal[i] * std::exp(phase);
        }
    }
    return harmonics;
}

auto CpuWorker::InverseDiscreteFourierTransform(const ComplexSignal& harmonics) -> Signal {
    const auto m = harmonics.size();
    const auto n = m;  // TODO: fix M size to half

    Signal signal(n, 0);
    for (unsigned int i = 0; i < n; i++) {
        auto& value = signal[i];

        for (unsigned int j = 0; j < m; j++) {
            const auto& harmonic = harmonics[j];

            float phase = 2 * std::numbers::pi * i * j / n - std::arg(harmonic);
            value += std::abs(harmonic) * std::cos(phase);
        }
    }
    return signal;
}

auto CpuWorker::FastFourierTransform(const Signal& signal) -> ComplexSignal {
    const auto initial_n = signal.size();
    const size_t n = std::pow(2, std::ceil(std::log2f(initial_n)));

    ComplexSignal harmonics(n);
    for (size_t i = 0; i < initial_n; i++) {
        harmonics[i] = signal[i];
    }
    GenericFastFourierTransform(harmonics, false);

    // TODO: fix harmonics size to half
    return harmonics;
}

void CpuWorker::GenericFastFourierTransform(Worker::ComplexSignal& harmonics, bool invert) const {
    using namespace std::complex_literals;

    const auto n = harmonics.size();
    for (unsigned int i = 0; i < n; i++) {
        unsigned int reverse_index = 0;
        for (unsigned int j = 1; j < n; j <<= 1) {
            reverse_index <<= 1;
            reverse_index += bool(i & j);
        }
        if (reverse_index < i) std::swap(harmonics[reverse_index], harmonics[i]);
    }

    const auto w_sign = invert ? -1 : 1;
    for (unsigned int m = 1; m <= n; m <<= 1) {
        const auto w_m_phase = w_sign * 2.0f * std::numbers::pi_v<float> / m * 1if;
        const auto w_m = std::exp(w_m_phase);

        for (unsigned int k = 0; k < n; k += m) {
            std::complex<float> w = 1;

            for (unsigned int j = 0; j < m / 2; j++, w *= w_m) {
                const auto index_even = k + j;
                const auto index_odd = k + j + m / 2;
                const auto& [harmonic_even, harmonic_odd] =
                    std::tie(harmonics[index_even], harmonics[index_odd]);

                std::tie(harmonic_even, harmonic_odd) = std::make_tuple(
                    harmonic_even + w * harmonic_odd, harmonic_even - w * harmonic_odd);
            }
        }
    }

    if (!invert)
        for (auto& harmonic : harmonics) {
            harmonic /= n;
        }
}

auto CpuWorker::InverseFastFourierTransform(const ComplexSignal& harmonics) -> Signal {
    const auto initial_n = harmonics.size();
    const size_t n = std::pow(2, std::ceil(std::log2(initial_n)));

    ComplexSignal complex_signal(n);
    for (size_t i = 0; i < initial_n; i++) {
        complex_signal[i] = harmonics[i];
    }
    GenericFastFourierTransform(complex_signal, true);

    Signal signal(n);
    for (size_t i = 0; i < n; i++) {
        signal[i] = complex_signal[i].real();
    }

    // TODO: fix harmonics size to half
    return signal;
}

}  // namespace fft_visualizer::model::worker