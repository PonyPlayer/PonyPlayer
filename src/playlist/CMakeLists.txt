project(playlist)
set(CPP_SOURCES
        kv_engine.cpp playlist.cpp controller.cpp info_accessor.cpp)

qt_add_library(${PROJECT_NAME} STATIC
        ${CPP_SOURCES})

target_sources(playlist PRIVATE
        ${CMAKE_SOURCE_DIR}/include/kv_engine.h
        ${CMAKE_SOURCE_DIR}/include/playlist.h
        ${CMAKE_SOURCE_DIR}/include/controller.h
        ${CMAKE_SOURCE_DIR}/include/info_accessor.h
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Quick
        Qt::Sql)

