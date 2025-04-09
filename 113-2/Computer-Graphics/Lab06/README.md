# Computer Graphics 電腦圖學

## Lab 05: Dual Viewport with Arbitrary Rotation 雙視窗任意軸旋轉

### 說明
本次實驗實作了雙視窗的 3D 空間物體任意軸旋轉功能，並結合平移與縮放操作，讓使用者可以透過鍵盤與滑鼠操作來控制物體的變換。

### 功能
1. **雙視窗顯示**：
   - 左右兩個視窗分別顯示不同的物體變換狀態。
   - 支援切換操作的活躍視窗。

2. **任意軸旋轉**：
   - 使用滑鼠點擊定義旋轉軸的兩個端點 `P` 和 `-P`。
   - 支援以自訂軸進行旋轉，或使用預設的 Z 軸。

3. **平移**：
   - 使用鍵盤控制物體在 X、Y、Z 軸方向上的平移。

4. **縮放**：
   - 支援物體的放大與縮小操作。

5. **重置**：
   - 可快速重置物體的變換狀態。

6. **退出**：
   - 支援快捷鍵退出程式。

### 操作說明
| 操作方式       | 功能                                                                 |
|----------------|----------------------------------------------------------------------|
| 滑鼠左鍵       | 定義旋轉軸的兩個端點 `P` 和 `-P`，以滑鼠點擊位置計算世界座標。         |
| `Tab`          | 切換活躍視窗（左或右）。                                             |
| `U` / `u`      | 順時針 / 逆時針旋轉角度增加或減少 5°。                               |
| `W` / `S`      | 向上 / 向下平移。                                                   |
| `A` / `D`      | 向左 / 向右平移。                                                   |
| `Q` / `E`      | 向前 / 向後平移。                                                   |
| `T` / `R`      | 放大 / 縮小物體。                                                   |
| `Space`        | 重置當前視窗的物體變換與旋轉軸設定。                                 |
| `X` / `Esc`    | 退出程式。                                                          |

### 程式架構
1. **雙視窗管理**：
   - 使用 OpenGL 的 `glViewport` 分割視窗。
   - 支援滑鼠點擊與鍵盤操作切換活躍視窗。

2. **矩陣運算**：
   - 包括旋轉矩陣、平移矩陣、縮放矩陣的生成與矩陣相乘。

3. **繪製功能**：
   - 繪製三軸（X、Y、Z）與自訂軸。
   - 繪製立方體作為變換的物體。

4. **輸入處理**：
   - 透過 `glutKeyboardFunc` 處理鍵盤輸入，實現變換操作。
   - 透過 `glutMouseFunc` 處理滑鼠點擊，計算旋轉軸。

5. **渲染與視窗管理**：
   - 使用 OpenGL 的 `glut` 函式庫進行場景渲染與視窗大小調整。

### 執行畫面
- **初始狀態**：
  - 左右視窗顯示三軸與立方體。
- **操作後**：
  - 立方體根據輸入進行旋轉、平移或縮放，並顯示自訂旋轉軸。

### 編譯與執行
1. **設定 CLion 專案**：
   - 在 CLion 中開啟專案資料夾。
   - 確保 `CMakeLists.txt` 已正確設定，範例如下：
     ```cmake
     cmake_minimum_required(VERSION 3.10)
     project(Lab05)

     set(CMAKE_CXX_STANDARD 11)

     find_package(OpenGL REQUIRED)
     find_package(GLUT REQUIRED)

     include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
     link_directories(${OPENGL_LIBRARY_DIRS} ${GLUT_LIBRARY_DIRS})

     add_executable(Lab05 Lab05.cpp)
     target_link_libraries(Lab05 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
     ```

2. **編譯**：
   - 點擊 CLion 的 **Build** 按鈕，或使用快捷鍵 `Command + F9`。

3. **執行**：
   - 點擊 CLion 的 **Run** 按鈕，或使用快捷鍵 `Control + R`。

### 注意事項
- 請確保已安裝 OpenGL 開發環境（如 `freeglut`）。
- 若無法正常執行，請檢查是否正確連結相關函式庫。

### 結論
本次實驗透過雙視窗的設計，結合矩陣運算與 OpenGL 的功能，實現了 3D 空間中物體的任意軸旋轉與多種變換操作，幫助理解 3D 變換的基本概念與視窗管理技術。