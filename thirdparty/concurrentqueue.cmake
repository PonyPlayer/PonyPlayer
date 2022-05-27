include(FetchContent)

if(NOT NO_SSH_KEY)
    set(concurrentqueue_GIT git@github.com:cameron314/concurrentqueue.git)
else()
    set(concurrentqueue_GIT https://github.com/cameron314/concurrentqueue.git)
endif()

FetchContent_Declare(concurrentqueue
    GIT_REPOSITORY ${concurrentqueue_GIT}
    GIT_TAG        22c78daf65d2c8cce9399a29171676054aa98807
)

FetchContent_MakeAvailable(concurrentqueue)

set_property(
    DIRECTORY ${concurrentqueue_SOURCE_DIR}
    APPEND
    PROPERTY COMPILE_OPTIONS -w
)