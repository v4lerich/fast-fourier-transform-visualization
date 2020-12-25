#ifndef FFT_VISUALIZER_APPLICATION_CONFIGURATION_H
#define FFT_VISUALIZER_APPLICATION_CONFIGURATION_H

#include <IconsForkAwesome.h>

#include <string>

namespace fft_visualizer::configuration {

enum class ApplicationTheme { kClassic, kLight, kDark };

struct OpenGlVersion {
    int major;
    int minor;
};

constexpr auto kWindowWidth = 1280;
constexpr auto kWindowHeight = 720;

constexpr auto kVsyncValue = -1;  // Adaptive VSYNC
constexpr auto kImGuiTheme = ApplicationTheme::kClassic;

constexpr auto kOpenGlVersion = OpenGlVersion{.major = 4, .minor = 2};
constexpr auto kGlslVersion = "#version 420";

const std::string kResourcePath = "./res/";
const std::string kRubikFontPath = kResourcePath + "fonts/RubikFont/Rubik-Regular.ttf";
const std::string kForkAwesomeFontPath =
    kResourcePath + "fonts/ForkAwesomeFont/" + FONT_ICON_FILE_NAME_FK;

}  // namespace fft_visualizer::configuration

#endif  // FFT_VISUALIZER_APPLICATION_CONFIGURATION_H
