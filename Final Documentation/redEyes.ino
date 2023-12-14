#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN_MATRIX_1 20
#define PIN_MATRIX_2 21
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

Adafruit_NeoMatrix matrix1 = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN_MATRIX_1,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN_MATRIX_2,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

void setup() {
  matrix1.begin();
  matrix2.begin();
  matrix1.setTextWrap(false);
  matrix2.setTextWrap(false);
  matrix1.setBrightness(40);
  matrix2.setBrightness(40);
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();
}

void loop() {
  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(1000);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

    // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 0, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 0, 30));
  matrix2.show();

  delay(1000);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();
}
