#ifndef FFT_VISUALIZER_VIEW_H
#define FFT_VISUALIZER_VIEW_H

#include <signal_generator/harmonic_signal_generator.h>
#include <signal_generator/polyharmonic_signal_generator.h>
#include <worker_model.h>

#include <random>
namespace fft_visualizer::view {

class View {
  public:
    View() = default;
    virtual ~View() = default;

    virtual void Render() = 0;
    virtual auto WantClose() -> bool;

  protected:
    static void CenterText(const std::string& text);

    void SetWantClose();

  private:
    bool want_close_{false};
};

}  // namespace fft_visualizer::view

#endif  // FFT_VISUALIZER_VIEW_H
