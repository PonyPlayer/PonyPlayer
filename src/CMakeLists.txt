qt_add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(view)
add_subdirectory(utils)
add_subdirectory(example)
add_subdirectory(ffmpeg)
add_subdirectory(tests)
include(${CMAKE_SOURCE_DIR}/cmake/platform.cmake)

target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt::Core
        PRIVATE Qt::Gui
        PRIVATE Qt::Quick
        PRIVATE Qt::Qml
        PRIVATE Qt::QmlWorkerScript
        PRIVATE ${FFmpeg}
        utils
        view
        )


