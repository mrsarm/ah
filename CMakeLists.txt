cmake_minimum_required(VERSION 3.10)
project(ah C)

enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out")

include_directories("${CMAKE_SOURCE_DIR}/src")

set(BASE_SOURCE_FILES
    src/freqlist.c
    src/util.c
    src/ah.c)

# Executable "ah"
add_executable(ah src/main.c
               ${BASE_SOURCE_FILES})

set(BASE_TEST_SOURCE_FILES
    test/util_t.c)

# Executable with unit tests "test_fq"
add_executable(test_ah test/test_ah.c
               ${BASE_TEST_SOURCE_FILES}
               ${BASE_SOURCE_FILES})

# Executable with unit tests "test_huff"
add_executable(test_huff test/test_huff.c
        ${BASE_TEST_SOURCE_FILES}
        ${BASE_SOURCE_FILES})

# Executable with unit tests "test_util"
add_executable(test_util test/test_util.c
        ${BASE_TEST_SOURCE_FILES}
        ${BASE_SOURCE_FILES})

# Install with `make install`
install(TARGETS ah
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)

# To trigger dependencies before `make test`
set_property(DIRECTORY APPEND
             PROPERTY TEST_INCLUDE_FILES
             "${CMAKE_CURRENT_BINARY_DIR}/BuildTestTarget.cmake")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/BuildTestTarget.cmake"
     "execute_process(COMMAND \"${CMAKE_COMMAND}\""
     " --build \"${CMAKE_BINARY_DIR}\""
     " --config \"\$ENV{CMAKE_CONFIG_TYPE}\")")

# Unit tests with `make test`
add_test(test_ah ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_ah)
add_test(test_huff ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_huff)
add_test(test_util ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_util)

# To clean everything: compiled binaries and *make* files
add_custom_target(clean-all
  make clean && rm -fR ${CMAKE_BINARY_DIR}/CMakeCache.txt
                       ${CMAKE_BINARY_DIR}/cmake_install.cmake
                       ${CMAKE_BINARY_DIR}/Makefile
                       ${CMAKE_BINARY_DIR}/CMakeFiles
                       ${CMAKE_BINARY_DIR}/cmake-build-debug
                       ${CMAKE_BINARY_DIR}/install_manifest.txt
                       ${CMAKE_BINARY_DIR}/BuildTestTarget.cmake
                       ${CMAKE_BINARY_DIR}/CTestTestfile.cmake
                       ${CMAKE_BINARY_DIR}/Testing
             && rmdir  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
