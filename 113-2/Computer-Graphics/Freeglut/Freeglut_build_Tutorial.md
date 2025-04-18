# Freeglut Setup Guide for CLion with MinGW

This guide is intended for beginners to help set up Freeglut for use with CLion's MinGW environment. Follow the steps below carefully to ensure proper configuration.

## Prerequisites
- **CLion IDE** installed on your system.
- The **MinGW** environment provided by CLion, typically located at:`C:\Program Files\JetBrains\CLion {version}\bin\mingw`.

Replace `{version}` with your installed version of CLion.
- The Freeglut compressed file (`freeglut-MinGW-3.0.0-1.mp.zip`).

## Steps to Set Up Freeglut

1. **Extract the Freeglut Archive**  
 Unzip the `freeglut-MinGW-3.0.0-1.mp.zip` file to a temporary location on your computer.

2. **Organize Files into MinGW Directories**  
 Inside the extracted Freeglut folder, you will find three main directories: `bin`, `include`, and `lib`. These need to be copied into the corresponding directories in your MinGW environment.

 - **`bin` Directory**  
   Copy all files from the `bin` directory of Freeglut into:
   ```
   C:\Program Files\JetBrains\CLion {version}\bin\mingw\bin
   ```

 - **`include` Directory**  
   Inside the `include` directory of Freeglut, there is a folder named `GL`. Copy the entire `GL` folder into:
   ```
   C:\Program Files\JetBrains\CLion {version}\bin\mingw\include
   ```

 - **`lib` Directory**  
   Copy all files from the `lib` directory of Freeglut into:
   ```
   C:\Program Files\JetBrains\CLion {version}\bin\mingw\lib
   ```

3. **Handle x64 Folder (If Present)**  
 If the extracted Freeglut files include an `x64` folder, you need to determine whether to use it based on your system's architecture:
 - For **64-bit systems**, use the files inside the `x64` folder.
 - For **32-bit systems**, ignore the `x64` folder and use the files from the main directories (`bin`, `include`, `lib`).

 Ensure you copy the appropriate files into the corresponding MinGW directories.

## Notes
- This setup is specifically for the MinGW environment bundled with [CLion](https://www.jetbrains.com/clion/promo/?source=google&medium=cpc&campaign=APAC_en_ASIA_Clion_Branded&term=clion&content=489240779651&gad_source=1&gclid=Cj0KCQjwqcO_BhDaARIsACz62vM64kCXdmxID8F7USdmngq7kVlNhcBpK-Eqy1M8J1zI85fGiZ-IiGsaAsbDEALw_wcB).
- Ensure that the paths are correct and that you do not overwrite any existing files unless necessary.
- If you encounter any issues, double-check that the files are placed in the correct directories.
- If you need to upgrade freeglut, you can download it from the [freeglut website](https://www.transmissionzero.co.uk/software/freeglut-devel/).

By following these steps, you should be able to successfully configure Freeglut for use with CLion's MinGW environment. Happy coding!

---

## CMakeLists.txt

```
cmake_minimum_required(VERSION 3.10)
project({Project_Name})

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    message(STATUS "Detected macOS")

    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)

    add_definitions(-DGL_SILENCE_DEPRECATION)

    add_executable({Project_Name} {main.cpp})

    target_link_libraries({Project_Name} 
        "-framework OpenGL"
        "-framework GLUT"
        "-framework Cocoa"
    )

elseif (WIN32)
    message(STATUS "Detected Windows with MinGW")

    include_directories(${PROJECT_SOURCE_DIR}/include)
    link_directories(${PROJECT_SOURCE_DIR}/lib)

    add_executable({Project_Name} {main.cpp})

    target_link_libraries({Project_Name}
        freeglut
        opengl32
        glu32
    )
endif()
```

### Notes:
- Replace `{Project_Name}` with the name of your project.
- Replace `{main.cpp}` with the name of your main source file.
- Ensure that the file paths and names match your project's structure.