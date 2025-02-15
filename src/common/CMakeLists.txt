cmake_minimum_required(VERSION 3.5)

file(GLOB COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(common SHARED ${COMMON_SOURCES})

# Include directories for other projects
target_include_directories(common PUBLIC ${CMAKE_SOURCE_DIR}/include)
