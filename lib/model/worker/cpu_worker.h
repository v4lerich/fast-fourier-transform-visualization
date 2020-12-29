#ifndef FFT_VISUALIZER_CPU_WORKER_H
#define FFT_VISUALIZER_CPU_WORKER_H

#include "worker.h"

namespace fft_visualizer::model::worker {

class CpuWorker : public Worker {
  public:
    auto DiscreteFourierTransform(const Signal& signal) -> ComplexSignal override;
    auto InverseDiscreteFourierTransform(const ComplexSignal& harmonics) -> Signal override;

    auto FastFourierTransform(const Signal& signal) -> ComplexSignal override;
    auto InverseFastFourierTransform(const ComplexSignal& signal) -> Signal override;

  private:
    void GenericFastFourierTransform(ComplexSignal& harmonics, bool invert) const;
};

}  // namespace fft_visualizer::model::worker

#endif  // FFT_VISUALIZER_CPU_WORKER_H
