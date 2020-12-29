project(fft-visualizer-resources)

add_custom_target(${PROJECT_NAME} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ ${RESOURCE_PATH}/shaders/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/kernels/ ${RESOURCE_PATH}/kernels/)

add_dependencies(${PROJECT_NAME}
        fork-awesome droidsans-font)
