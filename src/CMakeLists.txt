qt_add_executable(${PROJECT_NAME} main.cpp hotloader.hpp cursor.hpp ${PONY_ICON})
add_subdirectory(view)
add_subdirectory(utils)
add_subdirectory(decoder)
#add_subdirectory(tests)
add_subdirectory(player)
add_subdirectory(playlist)
add_subdirectory(wave)
add_subdirectory(audiosink)
#add_subdirectory(interpolate)
include(platform.cmake)


target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt::Core
        PRIVATE Qt::Gui
        PRIVATE Qt::Quick
        PRIVATE Qt::Qml
        PRIVATE Qt::QmlWorkerScript
        PRIVATE ${FFmpeg}
        utils
        decoder
        view
        playlist
        player
        audiosink
        lrctools
        wave
        #        interpolate
        )




