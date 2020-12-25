project(fft-visualizer)

add_executable(${PROJECT_NAME}
        application_configuration.h application.h application.cc
        main.cc)

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        imgui
        ${SDL2_LIBRARIES}
        glad2-loader
        fft-visualizer::view
        fft-visualizer::model
        icon-font-headers)

install(TARGETS ${PROJECT_NAME}
        DESTINATION bin)

add_dependencies(${PROJECT_NAME}
        fft-visualizer-resources
        fork-awesome)
