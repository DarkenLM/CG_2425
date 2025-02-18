# Project Name
set(SUBPROJECT_NAME common)

file(GLOB_RECURSE COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
file(GLOB_RECURSE COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")

message(STATUS "Common Sources Dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Common Sources: ${COMMON_SOURCES}")


if(WIN32)
    add_library(${SUBPROJECT_NAME} STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})
else()
    add_library(${SUBPROJECT_NAME} SHARED ${COMMON_SOURCES})
endif()




# Include directories for other projects
target_include_directories(${SUBPROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)



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



if(WIN32)
    foreach(SOURCE_FILE IN LISTS COMMON_SOURCES COMMON_HEADERS)
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
