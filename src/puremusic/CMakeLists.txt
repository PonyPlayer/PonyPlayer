project(puremusic)


qt_add_library(lyrics STATIC lyrics.cpp)

target_sources(lyrics PRIVATE
        ${CMAKE_SOURCE_DIR}/include/lyrics.h)


target_link_libraries(lyrics PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Multimedia
        Qt::Quick)