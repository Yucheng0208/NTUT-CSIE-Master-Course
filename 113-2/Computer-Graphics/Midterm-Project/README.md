# 3D OBJ 模型瀏覽器期中專案

本專案為國立臺北科技大學資訊工程相關課程的 **期中專案**，旨在透過 OpenGL 與 FreeGLUT 技術，建立一個具備互動式操作與多模型切換能力的 **3D 模型瀏覽器**。使用者可藉由鍵盤與滑鼠操作來旋轉、縮放、平移模型，並透過右鍵選單載入不同的 OBJ 模型進行觀察。此專案特別強調程式設計的結構性與互動性介面的設計。

---

## 專案技術說明

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

## 功能與操作說明

### 主要功能

- ✅ 模型載入：預設內建 4 個模型（茶壺、泰迪熊、葫蘆、八面體）
- ✅ 模型旋轉、平移與縮放操作（鍵盤控制）
- ✅ 三種渲染模式：點 / 線 / 面
- ✅ 切換隨機色彩模式
- ✅ 模型自動置中與等比例縮放顯示
- ✅ 畫面左下角顯示模型資訊（名稱、旋轉角度、深度）

### 鍵盤控制

| 鍵位 | 功能 |
|:----:|:-----|
| `W/S` | 上下旋轉 X 軸 |
| `A/D` | 左右旋轉 Y 軸 |
| `Q/E` | 順逆時針旋轉 Z 軸 |
| `Z/X` | 縮放模型 (Z 軸移動) |
| `R`   | 重置模型位置與角度 |
| `方向鍵` | 移動模型於 XY 平面 |
| `Page Up/Page Down` | 模型遠近前後調整 |

### 滑鼠右鍵選單功能

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

## 故障排除

在`MacOS`系統環境下的`CLion`將會發生運行後無法順利讀取`.obj`檔案，故遵循以下解決步驟：

1. 點選上方選單的`Run`。
2. 並在清單中找到`Edit Configurations`。
3. 進入後將`working directory`，改為`main.cpp`的目錄。

> [!Note]
> 對於這項問題產生的原因，主要與`macOS`運行時會去`cmake-build-debug` 的目錄中讀取檔案，接著進行運行，但是`.obj`並未在該目錄中，因此無法讀取，透過工作目錄重新導引後，就能解決。

### Bonus
使用以下方案，將會進行逐目錄找尋`models/`這個目錄，當找到後進行讀取與使用，懶人可以採取唷～

```cpp
#include <filesystem>
namespace fs = std::filesystem;

string getFullPath(const string& relativePath) {
    fs::path base = fs::current_path(); // 目前執行路徑（可能在 build 資料夾）

    // 一直往上層目錄找 models/
    while (!fs::exists(base / "models")) {
        base = base.parent_path();  // 往上一層走
        if (base.empty()) break;   // 如果已經到根目錄就停
    }

    return (base / relativePath).string();  // 回傳完整路徑
}
```