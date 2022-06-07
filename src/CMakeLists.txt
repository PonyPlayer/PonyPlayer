set(CPP_SOURCES
    main.cpp
    hotloader.hpp
    cursor.hpp
    crashreporter.hpp
)
if (WITH_WINDOWS_ICON)
    set(APP_ICON_RESOURCE "${CMAKE_SOURCE_DIR}/assets/ponyicon.rc")
    qt_add_executable(${PROJECT_NAME} WIN32 ${CPP_SOURCES} ${APP_ICON_RESOURCE})
else()
    set(APP_ICON_RESOURCE "${PONY_ICON}")
    qt_add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${APP_ICON_RESOURCE})
endif()


add_subdirectory(view)
add_subdirectory(utils)
add_subdirectory(decoder)
add_subdirectory(player)
add_subdirectory(playlist)
add_subdirectory(wave)
add_subdirectory(audiosink)

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
)
include(gtest.cmake)
project(PonyPlayer)



