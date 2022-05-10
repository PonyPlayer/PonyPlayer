project(unit_tests)
include(FetchContent)
FetchContent_Declare(gtest
        QUIET
        # mirror of https://github.com/google/googletest/archive/release-1.10.0.tar.gz
        URL https://scut-se-project.obs.cn-south-1.myhuaweicloud.com/googletest-release-1.10.0.tar.gz
        )
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)
add_executable(
        unit_tests
        example_test.cpp
        preview_test.cpp
)

target_link_libraries(unit_tests
        PRIVATE
        gtest_main
        example
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
