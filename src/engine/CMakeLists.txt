# Project Name
set(SUBPROJECT_NAME engine)

# Gather sources and headers
file(GLOB_RECURSE SUBPROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
file(GLOB_RECURSE SUBPROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" "**/*.h")

# Print debug information
message(STATUS "Engine Sources: ${SUBPROJECT_SOURCES}")

# Create the executable
add_executable(${SUBPROJECT_NAME} main.cpp ${SUBPROJECT_SOURCES} ${SUBPROJECT_HEADERS})

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

# Link against "common"
target_link_libraries(${SUBPROJECT_NAME} lib)
target_link_libraries(${SUBPROJECT_NAME} common)

# Include directories for other projects
target_include_directories(${SUBPROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

# ============== Find OpenGL and FreeGLUT ==============
set(OpenGL_GL_PREFERENCE "LEGACY")

# Include glew for vbo usage
find_package(GLEW REQUIRED)
target_link_libraries(${SUBPROJECT_NAME} GLEW::GLEW)

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


# Include OpenGL directories
target_include_directories(${SUBPROJECT_NAME} PUBLIC ${OpenGL_INCLUDE_DIRS})
target_link_directories(${SUBPROJECT_NAME} PUBLIC ${OpenGL_LIBRARY_DIRS})

if(WIN32)
	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
	
	# Check for GLUT
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")	
	
    # Include GLUT headers
    target_include_directories(${SUBPROJECT_NAME} PUBLIC ${TOOLKITS_FOLDER}/glut)

	# Link GLUT and OpenGL libraries
	target_link_libraries(
        ${SUBPROJECT_NAME} 
        ${OPENGL_LIBRARIES} 
		${TOOLKITS_FOLDER}/glut/glut32.lib
    )
	
    # Copy glut32.dll to the output directory
    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
    endif()
	
	# Set the startup project in Visual Studio
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${SUBPROJECT_NAME})
else () 
	#Linux and Mac
	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
	find_package(GLUT REQUIRED)

	message("LIBS: ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}")
    message("INCL: ${GLUT_INCLUDE_DIR}")
    message("LINK: ${GLUT_LIBRARY_DIRS}")
	
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})
	
	target_link_libraries(${SUBPROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )

	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)
endif(WIN32)
# ============== END Find OpenGL and FreeGLUT ==============

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