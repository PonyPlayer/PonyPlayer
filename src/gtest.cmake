project(unit_tests)
include(FetchContent)
if(NOT NO_SSH_KEY)
    set(gtest_GIT git@github.com:google/googletest.git)
else()
    set(gtest_GIT https://github.com/google/googletest.git)
endif()

FetchContent_Declare(gtest
        GIT_REPOSITORY ${gtest_GIT}
        GIT_TAG        release-1.11.0
        GIT_SHALLOW TRUE
        )

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

add_executable(
        unit_tests
        tests/example_test.cpp
        tests/decoder_test.cpp
        tests/frame_test.cpp
)

target_link_libraries(unit_tests
        PRIVATE
        gtest_main
        decoder
        Qt::Core
        Qt::Gui
        Qt::Quick
        )

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(unit_tests
        PROPERTIES
        LABELS "unit"
        DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
        240
    )
