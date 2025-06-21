# --- START OF FILE CMakeLists.txt (Corrected) ---

cmake_minimum_required(VERSION 3.10)
project(CG_FinalProject)

set(CMAKE_CXX_STANDARD 14)

# --- 1. 定義資源檔案列表 ---
# 將所有需要複製的資源檔案路徑集中管理

# TGA 紋理檔案列表 (路徑相對於專案根目錄)
set(TEXTURES
        TGA/bush.tga
        TGA/blackandwhiteTiles.tga
        TGA/pottedPlant.tga
        TGA/wood.tga
        TGA/sun.tga
)

# OBJ 模型檔案列表 (路徑相對於專案根目錄)
set(MODELS
        OBJ/bush.obj
        OBJ/diningTable.obj
        OBJ/pottedPlant.obj
        OBJ/rose.obj
        OBJ/sun.obj
        # 在這裡繼續添加你的其他 .obj 檔案
)

# --- 2. 建立執行檔 ---
# 這個指令在所有平台上都一樣，可以移到 if/else 區塊外面
add_executable(CG_FinalProject FinalProject.cpp)

# --- 3. 平台相關的設定 (函式庫、標頭檔路徑) ---
if (APPLE)
    message(STATUS "偵測到 macOS")

    # 設定 macOS 的 include 和 link 路徑
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
    add_definitions(-DGL_SILENCE_DEPRECATION)

    # 連結 macOS 專用的 Frameworks
    target_link_libraries(CG_FinalProject
            "-framework OpenGL"
            "-framework GLUT"
            "-framework Cocoa"
    )

elseif (WIN32)
    message(STATUS "偵測到 Windows")

    # 設定 Windows 的 include 和 link 路徑
    include_directories(${PROJECT_SOURCE_DIR}/include)
    link_directories(${PROJECT_SOURCE_DIR}/lib)

    # 連結 Windows 專用的函式庫
    target_link_libraries(CG_FinalProject
            freeglut
            opengl32
            glu32
    )
endif()

# --- 4. 通用的資源複製邏輯 ---
# 這段邏輯是跨平台的，因此放在 if/else 區塊外面，避免重複撰寫
# 它會在執行檔建置完成後，自動執行複製指令

# 首先，將紋理和模型的列表合併成一個總列表
set(RESOURCE_FILES ${TEXTURES} ${MODELS})

message(STATUS "準備將下列資源檔案加入建置後複製清單: ${RESOURCE_FILES}")

# 遍歷總列表中的每一個檔案
foreach(res_file ${RESOURCE_FILES})
    # 從完整路徑中僅取出檔案名稱 (例如: 從 "TGA/wood.tga" 得到 "wood.tga")
    get_filename_component(res_filename ${res_file} NAME)

    # 添加一個自訂命令 (Custom Command)
    # TARGET CG_FinalProject POST_BUILD: 表示在 "CG_FinalProject" 這個目標建置完成後執行
    add_custom_command(TARGET CG_FinalProject POST_BUILD
            # COMMAND: 要執行的命令
            # ${CMAKE_COMMAND} -E copy_if_different: CMake 內建的跨平台複製指令，只有在來源檔案較新時才會複製
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            # 來源檔案路徑 (專案根目錄下的相對路徑)
            "${CMAKE_SOURCE_DIR}/${res_file}"
            # 目標檔案路徑 ($<TARGET_FILE_DIR:...> 會自動解析為執行檔所在的目錄)
            "$<TARGET_FILE_DIR:CG_FinalProject>/${res_filename}"
            # COMMENT: 在建置時顯示的提示訊息
            COMMENT "正在複製資源 檔案: ${res_filename}"
    )
endforeach()

# --- END OF FILE CMakeLists.txt (Corrected) ---