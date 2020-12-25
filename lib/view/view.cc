#include "view.h"

namespace fft_visualizer::view {

auto View::WantClose() -> bool { return want_close_; }

void View::SetWantClose() { want_close_ = true; }

}  // namespace fft_visualizer::view
