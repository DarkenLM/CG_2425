cmake_minimum_required(VERSION 3.5)

add_executable(generator main.cpp)

# Link against "common"
target_link_libraries(generator PRIVATE common)

# Include directories for other projects
target_include_directories(generator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
