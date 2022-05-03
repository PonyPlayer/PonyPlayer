project(utils)

set(CPP_SOURCES
        logger.cpp
        blocking_queue.cpp
        countdown_latch.cpp
        helper.cpp
        )

qt_add_library(${PROJECT_NAME} STATIC
        ${CPP_SOURCES})

target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include/blocking_queue.h
        ${CMAKE_SOURCE_DIR}/include/countdown_latch.h
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Quick)

