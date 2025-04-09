cmake_minimum_required(VERSION 3.10)
project(CG_Lab06)

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    message(STATUS "Detected macOS")

    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)

    add_definitions(-DGL_SILENCE_DEPRECATION)

    add_executable(CG_Lab06 main.cpp)

    target_link_libraries(CG_Lab06
            "-framework OpenGL"
            "-framework GLUT"
            "-framework Cocoa"
    )

elseif (WIN32)
    message(STATUS "Detected Windows with MinGW")

    # Windows + MinGW，通常 OpenGL 內建在系統
    # 但是 freeglut 需要自己有 .lib 或 .a 檔案
    include_directories(${PROJECT_SOURCE_DIR}/include)
    link_directories(${PROJECT_SOURCE_DIR}/lib)

    add_executable(CG_Lab06 main.cpp)

    target_link_libraries(CG_Lab06
            freeglut
            opengl32
            glu32
    )
endif()
