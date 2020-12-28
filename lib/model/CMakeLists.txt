project(fft-visualizer-model)

add_library(${PROJECT_NAME}
        error.h error.cc
        worker.h worker.cc
        cpu_worker.h cpu_worker.cc
        open_cl_model.h open_cl_model.cc
        fft_visualizer_model.h fft_visualizer_model.cc
        signal_generator/signal_generator.h signal_generator/signal_generator.cc
        signal_generator/polygarmonic_signal_generator.h signal_generator/polygarmonic_signal_generator.cc
        signal_generator/garmonic_signal_generator.h signal_generator/garmonic_signal_generator.cc)

add_library(fft-visualizer::model
        ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
        PRIVATE ${OpenCL_LIBRARY})

target_include_directories(${PROJECT_NAME}
        PUBLIC . ${OpenCL_INCLUDE_DIR})
