project(fft-visualizer-view)

add_library(${PROJECT_NAME}
        view.h view.cc
        open_cl_info_view.h open_cl_info_view.cc
        fft_visualizer_view.h fft_visualizer_view.cc)

add_library(fft-visualizer::view
        ALIAS ${PROJECT_NAME})

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE icon-font-headers
        PUBLIC ${SDL2_LIBRARIES} imgui fft-visualizer::model)

target_include_directories(${PROJECT_NAME}
        PUBLIC . ${SDL2_INCLUDE_DIR})
