cmake_minimum_required(VERSION 3.10)
project(FinalProject)

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

# 🔥 這行可讓你在程式中抓 SOURCE_DIR
add_definitions(-DSOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_executable(FinalProject main.cpp)

# Include headers
target_include_directories(FinalProject PRIVATE
        /opt/homebrew/include
        ${OpenCV_INCLUDE_DIRS}
)

# 使用 macOS 原生 framework 替代 -lGLU / -lGL
target_link_libraries(FinalProject PRIVATE
        ${OpenCV_LIBRARIES}
        "-framework OpenGL"
        "-framework GLUT"
        "-framework Cocoa"
)
