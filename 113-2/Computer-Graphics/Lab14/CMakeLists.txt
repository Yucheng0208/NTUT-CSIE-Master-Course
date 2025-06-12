cmake_minimum_required(VERSION 3.10)
project(CG_Lab13)

set(CMAKE_CXX_STANDARD 14)

set(TEXTURES wood.tga grass.tga orb.tga)

if (APPLE)
    message(STATUS "Detected macOS")

    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)

    add_definitions(-DGL_SILENCE_DEPRECATION)

    add_executable(CG_Lab14 main.cpp
            main.cpp)

    target_link_libraries(CG_Lab14
            "-framework OpenGL"
            "-framework GLUT"
            "-framework Cocoa"
    )

    foreach(tex ${TEXTURES})
        add_custom_command(TARGET CG_Lab14 POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/${tex}
                $<TARGET_FILE_DIR:CG_Lab14>)
    endforeach()

elseif (WIN32)
    message(STATUS "Detected Windows with MinGW")

    include_directories(${PROJECT_SOURCE_DIR}/include)
    link_directories(${PROJECT_SOURCE_DIR}/lib)

    add_executable(CG_Lab14 main.cpp
            main.cpp)

    target_link_libraries(CG_Lab14
            freeglut
            opengl32
            glu32
    )

    foreach(tex ${TEXTURES})
        add_custom_command(TARGET CG_Lab14 POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/${tex}
                $<TARGET_FILE_DIR:CG_Lab13>)
    endforeach()
endif()
