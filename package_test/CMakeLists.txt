qt_add_executable(${PROJECT_NAME} package_test.cpp ${PONY_ICON})

include(${CMAKE_SOURCE_DIR}/src/platform.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core ${FFmpeg})




