project(playlist)
set(CPP_SOURCES kv_engine.cpp playlist.cpp controller.cpp info_accessor.cpp)

qt_add_library(${PROJECT_NAME} STATIC ${CPP_SOURCES})

target_sources(playlist PRIVATE
    include/kv_engine.h
    include/playlist.h
    include/controller.h
    include/info_accessor.h
)
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Sql
)

