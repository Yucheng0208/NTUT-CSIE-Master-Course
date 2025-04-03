cmake_minimum_required(VERSION 3.10)
project(CG_Lab02)

set(CMAKE_CXX_STANDARD 14)

# Include Homebrew directories for Apple Silicon
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

# Silence OpenGL deprecation warnings on macOS
add_definitions(-DGL_SILENCE_DEPRECATION)

add_executable(CG_Lab02 main.cpp)

# Link with frameworks properly on macOS
target_link_libraries(CG_Lab02
        "-framework OpenGL"
        "-framework GLUT"
        "-framework Cocoa"
)

# If the above still fails, use this alternative approach with freeglut
# Uncomment the line below
# target_link_libraries(CG_Lab01 -lfreeglut -framework OpenGL)