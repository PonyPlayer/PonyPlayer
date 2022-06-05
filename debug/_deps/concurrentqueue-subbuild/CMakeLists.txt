# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.22.0-rc2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(concurrentqueue-populate NONE)


# Pass through things we've already detected in the main project to avoid
# paying the cost of redetecting them again in ExternalProject_Add()
set(GIT_EXECUTABLE [==[/usr/bin/git]==])
set(GIT_VERSION_STRING [==[2.32.0 (Apple Git-132)]==])
set_property(GLOBAL PROPERTY _CMAKE_FindGit_GIT_EXECUTABLE_VERSION
  [==[/usr/bin/git;2.32.0 (Apple Git-132)]==]
)


include(ExternalProject)
ExternalProject_Add(concurrentqueue-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "git@github.com:cameron314/concurrentqueue.git" "GIT_TAG" "22c78daf65d2c8cce9399a29171676054aa98807"
                    SOURCE_DIR          "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-src"
                    BINARY_DIR          "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


