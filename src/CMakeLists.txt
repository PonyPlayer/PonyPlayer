qt_add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(view)
add_subdirectory(utils)
add_subdirectory(example)
add_subdirectory(ffmpeg)
add_subdirectory(tests)
add_subdirectory(render)
add_subdirectory(quickitem)
add_subdirectory(playlist)
add_subdirectory(config)
include(${CMAKE_SOURCE_DIR}/cmake/platform.cmake)


target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt::Core
        PRIVATE Qt::Gui
        PRIVATE Qt::Quick
        PRIVATE Qt::Qml
        PRIVATE Qt::QmlWorkerScript
        PRIVATE Qt::Widgets
        PRIVATE ${FFmpeg}
        ffmpeg
        utils
        view
        render
        quickitem
        playlist
        config)



