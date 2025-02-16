cmake_minimum_required(VERSION 3.5)

# Project Name
set(SUBPROJECT_NAME generator)

file(GLOB_RECURSE SUBPROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
file(GLOB_RECURSE SUBPROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" "**/*.h")
message(STATUS "Generator Sources: ${SUBPROJECT_SOURCES}")

add_executable(${SUBPROJECT_NAME} main.cpp ${SUBPROJECT_SOURCES})

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

# Link against "common"
target_link_directories(${SUBPROJECT_NAME} PRIVATE common)
target_link_libraries(${SUBPROJECT_NAME} PRIVATE common)

# Include directories for other projects
target_include_directories(${SUBPROJECT_NAME} PRIVATE ${SUBPROJECT_HEADERS} ${CMAKE_SOURCE_DIR}/include)
