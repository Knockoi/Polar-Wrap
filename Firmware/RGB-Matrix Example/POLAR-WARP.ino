#include <Adafruit_GFX.h>     // 包含 Adafruit_GFX 庫
#include <Adafruit_NeoMatrix.h>   // 包含 Adafruit_NeoMatrix 庫

#define PIN 6     // 定義連接 WS2812 矩陣的引腳為 6
#define MATRIX_WIDTH 32   // 定義矩陣寬度為 32 像素
#define MATRIX_HEIGHT 8    // 定義矩陣高度為 8 像素
#define CHAR_WIDTH 5   // 定義字符寬度為 5 像素
#define CHAR_HEIGHT 7    // 定義字符高度為 7 像素
#define CHAR_SPACING 2   // 定義字符之間的間隔為 2 像素
#define ANIM_DELAY 100   // 定義動畫延遲時間為 100 毫秒

// 創建一個 Adafruit_NeoMatrix 對象,用於控制 WS2812 矩陣
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN,
  NEO_MATRIX_TOP   + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

// 定義一個二維常量陣列,用於存儲 0-9 這 10 個數字的像素點陣列
const uint8_t charMap[10][CHAR_HEIGHT] = {
  {0b01110, 0b10001, 0b10011, 0b10001, 0b01110, 0b00000, 0b00000}, // 0
  {0b00100, 0b01100, 0b00100, 0b00100, 0b01110, 0b00000, 0b00000}, // 1
  {0b01110, 0b10001, 0b00010, 0b00100, 0b11111, 0b00000, 0b00000}, // 2
  {0b11111, 0b00010, 0b00100, 0b00001, 0b11111, 0b00000, 0b00000}, // 3
  {0b00010, 0b00110, 0b01010, 0b10001, 0b11111, 0b00000, 0b00000}, // 4
  {0b11111, 0b10000, 0b11110, 0b00001, 0b11110, 0b00000, 0b00000}, // 5
  {0b01110, 0b10001, 0b11111, 0b10001, 0b01110, 0b00000, 0b00000}, // 6
  {0b11111, 0b00001, 0b00010, 0b00100, 0b00100, 0b00000, 0b00000}, // 7
  {0b01110, 0b10001, 0b01110, 0b10001, 0b01110, 0b00000, 0b00000}, // 8
  {0b01110, 0b10001, 0b01111, 0b00001, 0b01110, 0b00000, 0b00000}  // 9
};

// 函數用於在矩陣上繪製一個字符
void drawChar(const uint8_t charMap[], int16_t x, int16_t y) {
  for (uint8_t i = 0; i < CHAR_HEIGHT; i++) {  // 遍歷字符的每一行
    for (uint8_t j = 0; j < CHAR_WIDTH; j++) { // 遍歷每一行的每一列
      // 根據 charMap 陣列中對應位置的值,在矩陣上繪製像素點(亮或暗)
      matrix.drawPixel(x + j, y + i, bitRead(charMap[i], j + (CHAR_WIDTH - 5)) * 0xFF0000);
    }
  }
}

// 函數用於在矩陣上繪製字符之間的間隔
void drawSpace(int16_t x, int16_t y) {
  for (uint8_t i = 0; i < CHAR_HEIGHT; i++) {  // 遍歷字符高度
    for (uint8_t j = 0; j < CHAR_SPACING; j++) { // 遍歷需要繪製的間隔寬度
      // 在指定位置繪製暗像素,產生間隔效果
      matrix.drawPixel(x + j + CHAR_WIDTH, y + i, 0);
    }
  }
}

// 函數用於實現數字變化時的動畫過渡效果
void animateTransition(int16_t x, int16_t y, uint8_t from, uint8_t to) {
  int8_t delta = (from < to) ? 1 : -1; // 根據 from 和 to 的大小,確定移動方向(上或下)
  for (uint8_t i = from; i != to; i += delta) { // 循環遍歷中間數字
    matrix.fillScreen(0);  // 清空矩陣
    drawChar(charMap[i], x, y); // 繪製當前數字
    matrix.show();  // 顯示當前幀
    delay(ANIM_DELAY);  // 延遲一段時間,產生動畫效果
    y += delta;  // 更新 y 坐標,實現上下移動
  }
  matrix.fillScreen(0);  // 清空矩陣
  drawChar(charMap[to], x, y);  // 繪製目標數字
  matrix.show();  // 顯示最終結果
}

void setup() {
  matrix.begin();   // 初始化矩陣
  matrix.setBrightness(40);  // 設置矩陣的亮度為 40
}

void loop() {
  int16_t x = 0, y = 0;  // 設置起始位置為 (0, 0)
  uint8_t value = 1;  // 初始數字為 1
  animateTransition(x, y, 0, value);  // 顯示數字 1
  delay(1000);  // 延遲 1 秒

  value = 3;  // 更改目標數字為 3
  animateTransition(x, y, 1, value);  // 從 1 過渡到 3
  delay(1000);  // 延遲 1 秒

  value = 5;  // 更改目標數字為 5
  animateTransition(x, y, 3, value);  // 從 3 過渡到 5
  delay(1000);  // 延遲 1 秒

  value = 3;  // 更改目標數字為 3
  animateTransition(x, y, 5, value);  // 從 5 過渡到 3
  delay(1000);  // 延遲 1 秒
}