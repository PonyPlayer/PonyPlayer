set(sonic_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/sonic)
add_subdirectory(${sonic_SOURCE_DIR})
set_property(
    DIRECTORY ${sonic_SOURCE_DIR}
    APPEND
    PROPERTY COMPILE_OPTIONS -w
)
include_directories(${sonic_SOURCE_DIR})