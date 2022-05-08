project(utils)

set(CPP_SOURCES
        logger.cpp
        countdown_latch.cpp
        )

qt_add_library(${PROJECT_NAME} STATIC
        ${CPP_SOURCES})


target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Quick)

