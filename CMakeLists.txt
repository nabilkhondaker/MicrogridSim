cmake_minimum_required(VERSION 3.15)
project(MicrogridSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Include Directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Gather Source Files recursively
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Executable
add_executable(microgrid_sim ${SRC_FILES})
target_compile_options(microgrid_sim PRIVATE -Wall -Wextra -Wpedantic -O3)
