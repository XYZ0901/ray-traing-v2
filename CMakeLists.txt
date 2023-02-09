cmake_minimum_required(VERSION 3.23)
project(rt2 VERSION 1.1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS )

include_directories(${PROJECT_SOURCE_DIR})

find_package(OpenCV REQUIRED)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

add_executable(rt2 main.cpp)

#target_link_libraries(rt2 ${CONAN_LIBS})