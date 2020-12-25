project(glad2-loader)

add_subdirectory(glad/cmake)
glad_add_library(glad_gl_core_42
        REPRODUCIBLE STATIC ALIAS
        LANGUAGE "c++"
        API gl:core=4.2)

find_package(SDL2 REQUIRED)
add_library(${PROJECT_NAME}
        STATIC gl_loader.h gl_loader.cc)

target_link_libraries(${PROJECT_NAME}
        PRIVATE ${SDL2_LIBRARY}
        PUBLIC glad_gl_core_42)

target_include_directories(${PROJECT_NAME}
        PUBLIC . ${SDL2_INCLUDE_DIR})
