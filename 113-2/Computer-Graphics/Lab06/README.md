# Clickable 2D Grid

此專案是一個使用 OpenGL 和 FreeGLUT 實作的 2D 點擊式格線應用程式。使用者可以透過滑鼠點擊格子來填色，並透過右鍵選單切換格線大小。

## 功能

1. **顯示格線**：預設為 10x10 的格線，格線顏色為白色。
2. **填色功能**：點擊格子後，該格子會被填色，(0,0) 格子為黃色，其餘格子為綠色。
3. **動態調整格線大小**：透過右鍵選單可切換格線大小為 10x10、15x15 或 20x20。
4. **右鍵選單**：
   - Grid 10x10
   - Grid 15x15
   - Grid 20x20
   - Exit (退出程式)

## 程式架構

- **初始化 (`init`)**：設定背景顏色、投影矩陣，並初始化預設填色格子。
- **畫格線 (`drawGrid`)**：繪製白色格線。
- **畫被填色的格子 (`drawFilledCells`)**：繪製已被點擊的格子，顏色依座標不同而異。
- **顯示 (`display`)**：清除畫面並繪製格線與填色格子。
- **滑鼠事件 (`mouse`)**：處理滑鼠點擊事件，將點擊的螢幕座標轉換為格子座標，並填色。
- **選單事件 (`selectFromMenu`)**：處理右鍵選單選項，切換格線大小或退出程式。
- **建立選單 (`buildPopupMenu`)**：建立右鍵選單並綁定選單事件。

## 使用方式
1. 左鍵點擊格子進行填色。
2. 右鍵開啟選單，選擇格線大小或退出程式。

## 程式碼重點

- 格線繪製

```
for (int i = -gridSize; i <= gridSize + 1; i++) {
    glVertex2f(i, -gridSize);
    glVertex2f(i, gridSize + 1);
    glVertex2f(-gridSize, i);
    glVertex2f(gridSize + 1, i);
}
```

- 螢幕座標轉格子座標
```
float normX = (float)x / windowWidth;
float normY = (float)(windowHeight - y) / windowHeight;
float worldX = normX * (2 * gridSize + 1) - gridSize;
float worldY = normY * (2 * gridSize + 1) - gridSize;
int gridX = (int)floor(worldX);
int gridY = (int)floor(worldY);
```

- 右鍵選單建立
```
int menu = glutCreateMenu(selectFromMenu);
glutAddMenuEntry("Grid 10x10", MENU_GRID_10);
glutAddMenuEntry("Grid 15x15", MENU_GRID_15);
glutAddMenuEntry("Grid 20x20", MENU_GRID_20);
glutAddMenuEntry("Exit", MENU_EXIT);
```