
cmake_minimum_required(VERSION 3.18.1)

project("edgedetection")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -frtti -fexceptions -O2")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/../../../jni/include)
include_directories(${CMAKE_SOURCE_DIR}/../../../gl)

find_package(OpenCV REQUIRED)

add_library(
    edgedetection
    SHARED
    native-lib.cpp
    opencv_processor.cpp
    opengl_renderer.cpp
)

target_link_libraries(
    edgedetection
    android
    log
    GLESv2
    EGL
    ${OpenCV_LIBS}
)

target_include_directories(edgedetection PRIVATE ${OpenCV_INCLUDE_DIRS})