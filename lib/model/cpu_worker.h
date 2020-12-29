#ifndef FFT_VISUALIZER_CPU_WORKER_H
#define FFT_VISUALIZER_CPU_WORKER_H

#include "worker.h"

namespace fft_visualizer::model {

class CpuWorker : public Worker {
  public:
    auto DiscreteFourierTransform(const Signal& signal) -> ComplexSignal override;
    auto InverseDiscreteFourierTransform(const ComplexSignal& harmonics) -> Signal override;

//    auto FastFourierTransform(const Signal& signal) -> ComplexSignal override;
//    auto InverseFastFourierTransform(const ComplexSignal& signal) -> Signal override;
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_CPU_WORKER_H
