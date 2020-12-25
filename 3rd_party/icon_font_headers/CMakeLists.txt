project(icon-font-headers
        LANGUAGES C)

add_library(${PROJECT_NAME}
        INTERFACE)

target_include_directories(${PROJECT_NAME}
        INTERFACE IconFontCppHeaders)
