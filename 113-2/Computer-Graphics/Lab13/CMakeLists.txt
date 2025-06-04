cmake_minimum_required(VERSION 3.10)
project(Lab12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    message(STATUS "Platform: macOS")

    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64")
        message(STATUS "Detected Apple Silicon (arm64)")
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/opencv")
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
    else()
        message(STATUS "Detected Intel Mac")
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/opencv")
        list(APPEND CMAKE_PREFIX_PATH "/usr/local")
    endif()

    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

find_package(OpenCV REQUIRED)

# 🔥 這行很重要
add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(Lab13 shadow.cpp)

if(OpenCV_FOUND)
    target_include_directories(Lab13 PRIVATE ${OpenCV_INCLUDE_DIRS})
endif()

target_link_libraries(Lab13 PRIVATE
        ${OpenCV_LIBRARIES}
        "-framework OpenGL"
        "-framework GLUT"
        "-framework Cocoa"
)

if(OpenCV_FOUND)
    message(STATUS "Found OpenCV version: ${OpenCV_VERSION}")
    message(STATUS "Include dirs: ${OpenCV_INCLUDE_DIRS}")
    message(STATUS "Libraries: ${OpenCV_LIBRARIES}")
endif()
