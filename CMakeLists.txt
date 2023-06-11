cmake_minimum_required(VERSION 3.5)

# Set project name
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Set build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)
# Set include directories
#include_directories(include)

# Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Set source files
file(GLOB SOURCES "src/*.cpp")

# Set debug and release flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")

# Set build options
option(BUILD_TESTS "Build tests" ON)

# Add executable
add_executable(game ${SOURCES})

# Add libraries if necessary
target_link_libraries(game ${SDL2_LIBRARIES} SDL2_mixer SDL2_image::SDL2_image)

# Add tests if necessary
if(BUILD_TESTS)
    # enable_testing()
    # add_subdirectory(tests)
endif()
