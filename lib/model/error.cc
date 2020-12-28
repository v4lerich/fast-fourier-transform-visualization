#include "error.h"

#include <utility>

namespace fft_visualizer::model {

Error::Error(const std::string& what_arg, std::string what_title)
    : std::runtime_error{what_arg}, what_title_{std::move(what_title)} {}

Error::Error(const char* what_arg, const char* what_title)
    : std::runtime_error{what_arg}, what_title_{what_title} {}

auto Error::WhatTitle() -> const char* const { return what_title_.c_str(); }

}  // namespace fft_visualizer::model
