cmake_minimum_required(VERSION 3.5)

# Project Name
set(SUBPROJECT_NAME engine)

file(GLOB_RECURSE SUBPROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
file(GLOB_RECURSE SUBPROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" "**/*.h")
message(STATUS "Engine Sources: ${SUBPROJECT_SOURCES}")
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
target_link_libraries(${SUBPROJECT_NAME} common)

# Include directories for other projects
target_include_directories(${SUBPROJECT_NAME} PRIVATE ${SUBPROJECT_HEADERS} ${CMAKE_SOURCE_DIR}/include)

# ============== Find OpenGL and FreeGLUT ==============
set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if(WIN32)
	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")	
	
	include_directories(${TOOLKITS_FOLDER}/glut )
	target_link_libraries(
        ${SUBPROJECT_NAME} 
        ${OPENGL_LIBRARIES} 
		${TOOLKITS_FOLDER}/glut/glut32.lib
    )
	
	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )	
	
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${SUBPROJECT_NAME})
else (WIN32) #Linux and Mac
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
