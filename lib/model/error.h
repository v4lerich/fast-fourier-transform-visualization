#ifndef FFT_VISUALIZER_ERROR_H
#define FFT_VISUALIZER_ERROR_H

#include <exception>
#include <stdexcept>

namespace fft_visualizer::model {

class Error : public std::runtime_error {
  public:
    explicit Error(const std::string& what_arg, std::string what_title = "Error");
    explicit Error(const char* what_arg, const char* what_title = "Error");
    Error(const Error& other) noexcept = default;

    auto WhatTitle() -> const char* const;

  private:
    std::string what_title_{"Error"};
};

}  // namespace fft_visualizer::model

#endif  // FFT_VISUALIZER_ERROR_H
