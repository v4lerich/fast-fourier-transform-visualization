#include <fft_visualizer_model.h>
#include <fft_visualizer_view.h>

#include <memory>

#include "application.h"

namespace view = fft_visualizer::view;
namespace model = fft_visualizer::model;

auto main(int argc, char **argv) -> int {
    fft_visualizer::Application application{argc, argv};

    auto return_code = application.Init();
    if (return_code != 0) {
        return return_code;
    }

    model::FftVisualizerModel model{};
    auto fft_visualizer = std::make_unique<view::FftVisualizerView>(model);
    application.AddView(std::move(fft_visualizer));

    return_code = application.Run();
    return return_code;
}
