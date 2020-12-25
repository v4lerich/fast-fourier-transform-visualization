#ifndef FFT_VISUALIZER_VIEW_H
#define FFT_VISUALIZER_VIEW_H

namespace fft_visualizer::view {

class View {
  public:
    View() = default;
    virtual ~View() = default;

    virtual void Render() = 0;
    virtual auto WantClose() -> bool;

  protected:
    void SetWantClose();

  private:
    bool want_close_{false};
};

}  // namespace fft_visualizer::view

#endif  // FFT_VISUALIZER_VIEW_H
