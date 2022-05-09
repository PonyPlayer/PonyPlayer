set(lrctools_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/lrctools)
add_subdirectory(${lrctools_SOURCE_DIR})
set_property(
        DIRECTORY ${lrctools_SOURCE_DIR}
        APPEND
        PROPERTY COMPILE_OPTIONS -w
)
include_directories("${lrctools_SOURCE_DIR}/include")