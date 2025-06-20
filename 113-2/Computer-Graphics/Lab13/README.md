# OpenGL Shadow Projection - FreeGLUT Version

本專案為一個使用 FreeGLUT 實作的簡單平面陰影繪製範例，改編自 Richard S. Wright Jr. 的 OpenGL SuperBible 教學內容，並依照作業要求進行了調整。

## 題目說明

- Modify the sourcce file `shadow.cpp` so that it can run without extra header files.
- Modify the scene:
    1. Place 4 different light sources in the scene and only 1 of them will be enable each time (the other three will be hidden)
        - The shadow will also change according to the postion of the light source.
    2. Draw the Jet in red.
    3. Draw the Jet shadow in dark red color
- Keyboad Control
    - Rotate the plane along center: (left-right & up-down)
    - Reset the plane
    - Select light sources: switch among 4 different light sources (Key: 1, 2, 3, 4)


## 功能說明

- 顯示一個簡單的飛機模型（Jet）與其在地面的陰影
- 支援多種光源位置（使用數字鍵 1–4 切換）
- 可透過方向鍵旋轉視角觀察飛機與陰影效果
- 使用自製的 `Math3D` 函式庫處理向量與矩陣運算

## 執行畫面

> 💡 預設將顯示紅色飛機，並投影陰影至地面，陰影顏色為暗紅色（紅色半透明）

## 控制說明

| 鍵位 | 功能            |
|------|---------------|
| ↑    | 向上旋轉物體        |
| ↓    | 向下旋轉物體        |
| ←    | 向左旋轉物體        |
| →    | 向右旋轉物體        |
| 1    | 切換至光源位置 1     |
| 2    | 切換至光源位置 2     |
| 3    | 切換至光源位置 3     |
| 4    | 切換至光源位置 4（預設） |
| r | 重置物體座標        |
| Esc  | 離開程式          |