project(fft-visualizer-model)

add_library(${PROJECT_NAME}
        fft_visualizer_model.h fft_visualizer_model.cc)

add_library(fft-visualizer::model
        ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
        PRIVATE glad2-loader ${OpenCL_LIBRARY})

target_include_directories(${PROJECT_NAME}
        PUBLIC . ${OpenCL_INCLUDE_DIR})
