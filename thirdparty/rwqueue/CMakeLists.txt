project(rwqueue CXX)
add_library(${PROJECT_NAME} INTERFACE atomicops.h readerwritercircularbuffer.h readerwriterqueue.h)
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE "${PROJECT_SOURCE_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)