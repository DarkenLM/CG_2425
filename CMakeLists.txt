if(WIN32)
    cmake_minimum_required(VERSION 3.10)
else()
    cmake_minimum_required(VERSION 3.5)
endif()

set(SRC_DIR "src")

PROJECT(CG_2425)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Default to Debug mode if no build type is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(WIN32)
    # Windows-specific output directories
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/out/debug/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/out/debug/lib/shared")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/out/debug/lib/static")
    
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/out/release/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/out/release/lib/shared")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/out/release/lib/static")

    # Set PDB output directory for libraries
    set(CMAKE_PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/out/debug/pdb")
    set(CMAKE_PDB_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/out/release/pdb")

    # Automatically collect all .cpp and .h files in the src directory and its subdirectories
    file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

    # Organize files into source groups based on their directory structure
    foreach(SOURCE_FILE IN LISTS SOURCES)
        # Get the relative path of the file from the src directory
        file(RELATIVE_PATH RELATIVE_PATH "${CMAKE_SOURCE_DIR}/src" "${SOURCE_FILE}")
        
        # Get the directory of the file (relative to src)
        get_filename_component(SOURCE_DIR "${RELATIVE_PATH}" DIRECTORY)
        
        # Convert forward slashes to backslashes for Windows IDE (e.g., Visual Studio)
        if(WIN32)
            string(REPLACE "/" "\\" SOURCE_DIR "${SOURCE_DIR}")
        endif()
        
        # Add the file to the corresponding source group
        source_group("${SOURCE_DIR}" FILES "${SOURCE_FILE}")
    endforeach()

else()
    # Linux-specific output directories
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/lib")
endif()

# Add subdirectories
add_subdirectory("${SRC_DIR}/lib")
add_subdirectory("${SRC_DIR}/common")
add_subdirectory("${SRC_DIR}/engine")
add_subdirectory("${SRC_DIR}/generator")

# Custom targets to run executables
if(WIN32)

else()
    add_custom_target(run_engine
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine
        DEPENDS engine
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Running the engine executable"
    )

    add_custom_target(run_generator
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/generator
        DEPENDS generator
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Running the generator executable"
    )
endif()