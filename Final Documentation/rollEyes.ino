#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 41
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.fillScreen(0);
  matrix.show();
}

void loop() {

  // Draw the outer rectangle
  matrix1.drawRect(1, 1, 6, 6, matrix.Color(255, 120, 30)); // Draw the outer rectangle
  // Draw the blinking inner rectangle
  matrix1.drawRect(3, 4, 2, 2, matrix.Color(255, 120, 30)); // Draw the blinking inner rectangle
  matrix1.show();

    // Draw the outer rectangle
  matrix2.drawRect(1, 1, 6, 6, matrix.Color(255, 120, 30)); // Draw the outer rectangle
  // Draw the blinking inner rectangle
  matrix2.drawRect(3, 4, 2, 2, matrix.Color(255, 120, 30)); // Draw the blinking inner rectangle
  matrix2.show();
  
  delay(1000);

  matrix1.fillScreen(0); // Clear the matrix
  matrix2.fillScreen(0); // Clear the matrix
  matrix1.show();
  matrix2.show();

  // Draw the outer rectangle
  matrix1.drawRect(1, 1, 6, 6, matrix.Color(255, 120, 30)); // Draw the outer rectangle
  // Draw the blinking inner rectangle
  matrix1.drawRect(4, 3, 2, 2, matrix.Color(255, 120, 30)); // Draw the blinking inner rectangle
  matrix1.show();

  // Draw the outer rectangle
  matrix2.drawRect(1, 1, 6, 6, matrix.Color(255, 120, 30)); // Draw the outer rectangle
  // Draw the blinking inner rectangle
  matrix2.drawRect(4, 3, 2, 2, matrix.Color(255, 120, 30)); // Draw the blinking inner rectangle
  matrix2.show();
  delay(1000);
  
  matrix1.fillScreen(0); // Clear the matrix
  matrix2.fillScreen(0); // Clear the matrix
  matrix1.show();
  matrix2.show();
  delay(10);

  // Draw the outer rectangle
  matrix1.drawRect(1, 1, 6, 6, matrix.Color(255, 120, 30)); // Draw the outer rectangle
  // Draw the blinking inner rectangle
  matrix1.drawRect(3, 2, 2, 2, matrix.Color(255, 120, 30)); // Draw the blinking inner rectangle
  matrix1.show();

  // Draw the outer rectangle
  matrix2.drawRect(1, 1, 6, 6, matrix.Color(255, 120, 30)); // Draw the outer rectangle
  // Draw the blinking inner rectangle
  matrix2.drawRect(3, 2, 2, 2, matrix.Color(255, 120, 30)); // Draw the blinking inner rectangle
  matrix2.show();
  delay(1000);
  
  matrix1.fillScreen(0); // Clear the matrix
  matrix2.fillScreen(0); // Clear the matrix
  matrix1.show();
  matrix2.show();

}
