project(networking)


qt_add_library(${PROJECT_NAME} STATIC netfind.cpp broadcast.cpp endpoint.cpp)

target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include/broadcast.h
        ${CMAKE_SOURCE_DIR}/include/endpoint.h
        ${CMAKE_SOURCE_DIR}/include/netfind.h
        )

target_link_libraries(${PROJECT_NAME} Qt::Network)


