# 3D OBJ 模型瀏覽器期中專案

本專案為國立臺北科技大學資訊工程相關課程的 **期中專案**，旨在透過 OpenGL 與 FreeGLUT 技術，建立一個具備互動式操作與多模型切換能力的 **3D 模型瀏覽器**。使用者可藉由鍵盤與滑鼠操作來旋轉、縮放、平移模型，並透過右鍵選單載入不同的 OBJ 模型進行觀察。此專案特別強調程式設計的結構性與互動性介面的設計。

---

## 🔧 專案技術說明

- **程式語言**：C++
- **圖形函式庫**：OpenGL + FreeGLUT
- **模型格式**：支援 Wavefront `.obj`
- **互動介面**：鍵盤操作 + 滑鼠右鍵選單
- **專案檔案結構**：
  ```
  📁 models/           // 模型資料夾（需放置 .obj 模型檔）
  └── main.cpp         // 主程式，包含模型讀取與渲染邏輯
  ```

---

## 🎮 功能與操作說明

### 🧩 主要功能

- ✅ 模型載入：預設內建 4 個模型（茶壺、泰迪熊、葫蘆、八面體）
- ✅ 模型旋轉、平移與縮放操作（鍵盤控制）
- ✅ 三種渲染模式：點 / 線 / 面
- ✅ 切換隨機色彩模式
- ✅ 模型自動置中與等比例縮放顯示
- ✅ 畫面左下角顯示模型資訊（名稱、旋轉角度、深度）

### 🕹 鍵盤控制

| 鍵位 | 功能 |
|:----:|:-----|
| `W/S` | 上下旋轉 X 軸 |
| `A/D` | 左右旋轉 Y 軸 |
| `Q/E` | 順逆時針旋轉 Z 軸 |
| `Z/X` | 縮放模型 (Z 軸移動) |
| `R`   | 重置模型位置與角度 |
| `方向鍵` | 移動模型於 XY 平面 |
| `Page Up/Page Down` | 模型遠近前後調整 |

### 📋 滑鼠右鍵選單功能

- **Model**
  - Load Teapot
  - Load Teddy
  - Load Gourd
  - Load Octahedron
- **Render Mode**
  - Render: Point
  - Render: Line
  - Render: Face
- **Color Mode**
  - Toggle Random Color
- **Clear Model**
- **Exit**

---

## 🛠 編譯與執行說明

### 系統需求

- 支援 OpenGL 的作業系統（Windows / Linux / macOS）
- 已安裝 `freeglut` 函式庫

### 編譯指令範例（g++）

```bash
g++ main.cpp -o viewer -lfreeglut -lopengl32 -lglu32
./viewer
```

### 注意事項

- 模型檔（`.obj`）請放置於 `models/` 資料夾下，與可執行檔或 `main.cpp` 同層。
- 此專案未處理 `.mtl` 材質與貼圖載入。