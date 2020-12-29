project(implot)

add_library(${PROJECT_NAME}
        STATIC
        implot/implot.h implot/implot.cpp
        implot/implot_internal.h
        implot/implot_items.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC implot .)

target_link_libraries(${PROJECT_NAME}
        PUBLIC imgui)
