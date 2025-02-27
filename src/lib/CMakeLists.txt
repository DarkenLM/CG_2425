# Project Name
set(SUBPROJECT_NAME lib)

file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
file(GLOB_RECURSE LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")

message(STATUS "Vendor Lib Sources Dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Vendor Lib Sources: ${LIB_SOURCES}")


if(WIN32)
    add_library(${SUBPROJECT_NAME} STATIC ${LIB_SOURCES} ${LIB_HEADERS})
else()
    add_library(${SUBPROJECT_NAME} SHARED ${LIB_SOURCES})
endif()




# Include directories for other projects
target_include_directories(${SUBPROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)



# Add debug-specific compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("DEBUG")
    target_compile_definitions(
        ${SUBPROJECT_NAME} PUBLIC
        $<$<CONFIG:Debug>:DEBUG_MODE>    # Define DEBUG_MODE macro in Debug mode
    )
else()
    # message("NODEBUG")
endif()



if(WIN32)
    foreach(SOURCE_FILE IN LISTS LIB_SOURCES LIB_HEADERS)
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
