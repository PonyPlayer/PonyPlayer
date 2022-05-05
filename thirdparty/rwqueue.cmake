set(rwqueue_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/rwqueue)
add_subdirectory(${rwqueue_SOURCE_DIR})
set_property(
        DIRECTORY ${rwqueue_SOURCE_DIR}
        APPEND
        PROPERTY COMPILE_OPTIONS -w
)
include_directories(${rwqueue_SOURCE_DIR})