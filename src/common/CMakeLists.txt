cmake_minimum_required(VERSION 3.5)

# Project Name
set(SUBPROJECT_NAME common)

file(GLOB_RECURSE COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
file(GLOB_RECURSE COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
message(STATUS "Common Sources Dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Common Sources: ${COMMON_SOURCES}")
add_library(${SUBPROJECT_NAME} SHARED ${COMMON_SOURCES})

# Include directories for other projects
target_include_directories(${SUBPROJECT_NAME} PUBLIC ${COMMON_HEADERS} ${CMAKE_SOURCE_DIR}/include)

# Add debug-specific compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("DEBUG")
    target_compile_definitions(
        ${SUBPROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE>    # Define DEBUG_MODE macro in Debug mode
    )
else()
    # message("NODEBUG")
endif()
