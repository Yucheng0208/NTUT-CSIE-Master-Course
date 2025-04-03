# Computer Graphics 電腦圖學

## Lab 04: Arbitrary Rotation 任意軸旋轉

### 說明
本次實驗實作了 3D 空間中物體的任意軸旋轉功能，並結合平移與縮放操作，讓使用者可以透過鍵盤操作來控制物體的變換。

### 功能
1. **任意軸旋轉**：
   - 使用者可透過鍵盤輸入兩個點 `v1` 和 `v2` 定義旋轉軸。
   - 支援以自訂軸進行旋轉，或使用預設的 Z 軸。

2. **平移**：
   - 使用鍵盤控制物體在 X、Y、Z 軸方向上的平移。

3. **縮放**：
   - 支援物體的放大與縮小操作。

4. **重置**：
   - 可快速重置物體的變換狀態。

5. **退出**：
   - 支援快捷鍵退出程式。

### 操作說明
| 鍵盤按鍵 | 功能                     |
|----------|--------------------------|
| `U`      | 順時針旋轉角度增加 0.5°  |
| `u`      | 逆時針旋轉角度減少 0.5°  |
| `W`      | 向上平移                 |
| `S`      | 向下平移                 |
| `A`      | 向左平移                 |
| `D`      | 向右平移                 |
| `Q`      | 向後平移                 |
| `E`      | 向前平移                 |
| `T`      | 放大                     |
| `R`      | 縮小                     |
| `I`      | 輸入自訂旋轉軸 `v1` 和 `v2` |
| `Space`  | 重置所有變換             |
| `X`      | 退出程式                 |

### 程式架構
1. **矩陣運算**：
   - 包括旋轉矩陣、平移矩陣、縮放矩陣的生成與矩陣相乘。

2. **繪製功能**：
   - 繪製三軸（X、Y、Z）與自訂軸。
   - 繪製立方體作為變換的物體。

3. **輸入處理**：
   - 透過 `glutKeyboardFunc` 處理鍵盤輸入，實現變換操作。

4. **渲染與視窗管理**：
   - 使用 OpenGL 的 `glut` 函式庫進行場景渲染與視窗大小調整。

### 執行畫面
- **初始狀態**：
  - 顯示三軸與立方體。
- **操作後**：
  - 立方體根據輸入進行旋轉、平移或縮放。

### 編譯與執行
1. **設定 CLion 專案**：
   - 在 CLion 中開啟專案資料夾。
   - 確保 `CMakeLists.txt` 已正確設定，範例如下：
     ```cmake
     cmake_minimum_required(VERSION 3.10)
     project(Lab04)

     set(CMAKE_CXX_STANDARD 11)

     find_package(OpenGL REQUIRED)
     find_package(GLUT REQUIRED)

     include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
     link_directories(${OPENGL_LIBRARY_DIRS} ${GLUT_LIBRARY_DIRS})

     add_executable(Lab04 Lab04.cpp)
     target_link_libraries(Lab04 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
     ```

2. **編譯**：
   - 點擊 CLion 的 **Build** 按鈕，或使用快捷鍵 `Command + F9`。

3. **執行**：
   - 點擊 CLion 的 **Run** 按鈕，或使用快捷鍵 `Control + R`。

### 注意事項
- 請確保已安裝 OpenGL 開發環境（如 `freeglut`）。
- 若無法正常執行，請檢查是否正確連結相關函式庫。

### 結論
本次實驗透過矩陣運算與 OpenGL 的結合，實現了 3D 空間中物體的任意軸旋轉，並提供了豐富的操作功能，幫助理解 3D 變換的基本概念。

