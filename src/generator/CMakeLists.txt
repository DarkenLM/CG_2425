# Project Name
set(SUBPROJECT_NAME generator)

file(GLOB_RECURSE SUBPROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
file(GLOB_RECURSE SUBPROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" "**/*.h")

message(STATUS "Generator Sources: ${SUBPROJECT_SOURCES}")

add_executable(${SUBPROJECT_NAME} main.cpp ${SUBPROJECT_SOURCES} ${SUBPROJECT_HEADERS})

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
target_link_libraries(${SUBPROJECT_NAME} PRIVATE common)

# Include directories for other projects
target_include_directories(${SUBPROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

# Organize files in the IDE (Windows-specific)
if(WIN32)
    foreach(SOURCE_FILE IN LISTS SUBPROJECT_SOURCES SUBPROJECT_HEADERS)
        # Get the relative path of the file from the current source directory
        file(RELATIVE_PATH RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${SOURCE_FILE}")
        
        # Get the directory of the file (relative to the current source directory)
        get_filename_component(SOURCE_DIR "${RELATIVE_PATH}" DIRECTORY)
        
        # Convert forward slashes to backslashes for Windows IDE (e.g., Visual Studio)
        string(REPLACE "/" "\\" SOURCE_DIR "${SOURCE_DIR}")
        
        # Add the file to the corresponding source group
        source_group("${SOURCE_DIR}" FILES "${SOURCE_FILE}")
    endforeach()
endif()