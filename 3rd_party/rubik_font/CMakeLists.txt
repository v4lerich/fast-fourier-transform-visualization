project(rubik-font)

add_custom_target(${PROJECT_NAME} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/rubik/fonts/ttf/Rubik-Regular.ttf ${RESOURCE_PATH}/fonts/RubikFont/Rubik-Regular.ttf)
