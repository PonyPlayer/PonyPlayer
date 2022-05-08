include(FetchContent)

FetchContent_Declare(concurrentqueue
        GIT_REPOSITORY git@github.com:cameron314/concurrentqueue.git
        GIT_TAG        22c78daf65d2c8cce9399a29171676054aa98807
        )

OPTION(PA_BUILD_SHARED OFF)
FetchContent_MakeAvailable(concurrentqueue)

set_property(
        DIRECTORY ${concurrentqueue_SOURCE_DIR}
        APPEND
        PROPERTY COMPILE_OPTIONS -w
)