project(fork-awesome)

add_custom_target(${PROJECT_NAME} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Fork-Awesome/fonts/forkawesome-webfont.ttf ${RESOURCE_PATH}/fonts/forkawesome-webfont.ttf)
