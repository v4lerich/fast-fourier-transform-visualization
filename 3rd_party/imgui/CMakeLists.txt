project(imgui)

add_library(${PROJECT_NAME}
        STATIC
        imguiconfig.h
        imgui/imgui.h
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_tables.cpp
        imgui/backends/imgui_impl_opengl3.h
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_sdl.h
        imgui/backends/imgui_impl_sdl.cpp)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2
        PUBLIC IMGUI_DEFINE_MATH_OPERATORS IMGUI_USER_CONFIG="./imguiconfig.h")

target_include_directories(${PROJECT_NAME}
        PUBLIC imgui . ${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
        PUBLIC ${SDL2_LIBRARY} glad2-loader)
