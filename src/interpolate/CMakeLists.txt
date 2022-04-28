project(interpolate)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

qt_add_library(${PROJECT_NAME} STATIC interpolate.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )

