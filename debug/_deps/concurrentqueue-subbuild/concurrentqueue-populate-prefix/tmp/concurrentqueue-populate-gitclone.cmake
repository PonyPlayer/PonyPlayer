
if(NOT "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-subbuild/concurrentqueue-populate-prefix/src/concurrentqueue-populate-stamp/concurrentqueue-populate-gitinfo.txt" IS_NEWER_THAN "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-subbuild/concurrentqueue-populate-prefix/src/concurrentqueue-populate-stamp/concurrentqueue-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-subbuild/concurrentqueue-populate-prefix/src/concurrentqueue-populate-stamp/concurrentqueue-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --config "advice.detachedHead=false" "git@github.com:cameron314/concurrentqueue.git" "concurrentqueue-src"
    WORKING_DIRECTORY "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'git@github.com:cameron314/concurrentqueue.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout 22c78daf65d2c8cce9399a29171676054aa98807 --
  WORKING_DIRECTORY "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '22c78daf65d2c8cce9399a29171676054aa98807'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-subbuild/concurrentqueue-populate-prefix/src/concurrentqueue-populate-stamp/concurrentqueue-populate-gitinfo.txt"
    "/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-subbuild/concurrentqueue-populate-prefix/src/concurrentqueue-populate-stamp/concurrentqueue-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/aaronpeng/Desktop/SE/PP/PonyPlayer/debug/_deps/concurrentqueue-subbuild/concurrentqueue-populate-prefix/src/concurrentqueue-populate-stamp/concurrentqueue-populate-gitclone-lastrun.txt'")
endif()

