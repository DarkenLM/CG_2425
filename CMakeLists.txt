cmake_minimum_required(VERSION 3.5)

set(SRC_DIR "src")

# Main Project Name
PROJECT(CG_2425)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug mode if no build type is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin)  # Executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib)  # Shared libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib)  # Static libraries

add_subdirectory("${SRC_DIR}/common")
add_subdirectory("${SRC_DIR}/engine")
add_subdirectory("${SRC_DIR}/generator")

add_custom_target(run_engine
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine
    DEPENDS engine
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin
)

add_custom_target(run_generator
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/generator
    DEPENDS generator
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin
)
