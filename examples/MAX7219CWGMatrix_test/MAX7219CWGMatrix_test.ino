/*
 * File:      MAX7219CWGMatrix_test.h
 * Authors:   Luke de Munk
 *
 * Example file to test the MAX7219CWGMatrix library. For more info, checkout:
 * https://github.com/LukedeMunk/ESP32-8x8ledmatrix-big-display
 */
#include "MAX7219CWGMatrix.h"

/* Pins */
#define CLOCK_OUT_PIN   18                                                  //Use hardware SPI GPIO clock pin for your hardware
#define DATAIN_PIN      23                                                  //Use hardware SPI GPIO data pin for your hardware
#define CS_PIN          14

#define WIDTH           4                                                   //4 segments horizontal
#define HEIGHT          3                                                   //3 segments vertical

MAX7219CWGMatrix matrix(WIDTH, HEIGHT, CS_PIN);                             //Create a MAX7219CWGMatrix object

/**************************************************************************/
/*!
  @brief    Setup the controller.
*/
/**************************************************************************/
void setup() {
    /* Initialise serial communication */
    Serial.begin(115200);

    /* Initialise the display */
    matrix.setPower(true);
    matrix.setIntensity(0);                                                 //0 = lowest, 15 = highest
    matrix.setRotation(UPSIDE_DOWN_ROTATION);
    matrix.display();
    
    delay(100);

    debugln("Demo with examples started");
}

/**************************************************************************/
/*!
  @brief    Main loop containing example functions.
*/
/**************************************************************************/
void loop() {
    debugln("Demo pixel function");
    demoPixels();
    debugln("Demo line functions");
    demoLines();
    debugln("Demo rectangle function");
    demoRectangles();
    debugln("Demo circle function");
    demoCircles();
    debugln("Demo triangle function");
    demoTriangles();
    debugln("Demo string function");
    demoStrings();
}

/**************************************************************************/
/*!
  @brief    An example of the pixel drawer.
*/
/**************************************************************************/
void demoPixels() {
    for (uint8_t i = 0; i < 50; i++) {
        matrix.drawPixel(random(0, matrix.getWidth()), random(0, matrix.getHeight()), 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }
}

/**************************************************************************/
/*!
  @brief    An example of the line drawers.
*/
/**************************************************************************/
void demoLines() {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t y = 0; y < matrix.getHeight(); y++) {
            matrix.drawLine(0, y, matrix.getWidth(), matrix.getHeight()-y, 1);
            matrix.drawLine(0, matrix.getHeight()-y, matrix.getWidth(), y, 1);
            matrix.display();
            delay(100);
            matrix.clear();
        }
    }

    for (uint8_t x = 0; x < matrix.getWidth(); x++) {
        matrix.drawVLine(x, 0, random(1, matrix.getHeight()), 1);
        matrix.display();
        delay(50);
    }

    delay(500);
    matrix.clear();

    for (uint8_t y = 0; y < matrix.getHeight(); y++) {
        matrix.drawHLine(0, y, random(1, matrix.getWidth()), 1);
        matrix.display();
        delay(50);
    }
    delay(500);
    matrix.clear();
}

/**************************************************************************/
/*!
  @brief    An example of the rectangle drawers.
*/
/**************************************************************************/
void demoRectangles() {
    uint8_t w = 0;
    uint8_t h = 0;

    while (w < matrix.getWidth() || h < matrix.getHeight()) {
        if (w > matrix.getWidth()-1) {
            w = matrix.getWidth()-1;
        }

        if (h > matrix.getHeight()-1) {
            h = matrix.getHeight()-1;
        }

        matrix.drawFillRect(0, 0, w, h, 1);
        matrix.display();
        delay(100);
        w++;
        h++;
    }

    while (w > 0 || h > 0) {
        matrix.drawRect(0, 0, w, h, 1);
        matrix.display();
        delay(100);
        matrix.clear();

        if (w > 0) {
            w--;
        }

        if (h > 0) {
            h--;
        }
    }
}

/**************************************************************************/
/*!
  @brief    An example of the circle drawers.
*/
/**************************************************************************/
void demoCircles() {
    for (uint8_t r = 0; r < 9; r++) {
        matrix.drawCircle(matrix.getWidth()/2, matrix.getHeight()/2, r, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }

    for (uint8_t r = 9; r > 0; r--) {
        matrix.drawCircle(matrix.getWidth()/2, matrix.getHeight()/2, r, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }

    for (uint8_t r = 0; r < 9; r++) {
        matrix.drawFillCircle(matrix.getWidth()/2, matrix.getHeight()/2, r, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }
    
    for (uint8_t r = 9; r > 0; r--) {
        matrix.drawFillCircle(matrix.getWidth()/2, matrix.getHeight()/2, r, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }
}

/**************************************************************************/
/*!
  @brief    An example of the triangle drawers.
*/
/**************************************************************************/
void demoTriangles() {
    for (uint8_t x = 5; x < 5+8; x++) {
        matrix.drawTriangle(x, 3, matrix.getWidth()-2, 10, 3, 8, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }
    
    for (uint8_t y = 10; y < 10+10; y++) {
        matrix.drawTriangle(5+8, 3, matrix.getWidth()-2, y, 3, 8, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }

    for (uint8_t x = 3; x < 3+6; x++) {
        matrix.drawTriangle(5+8, 3, matrix.getWidth()-2, 10+10, x, 8, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }


    for (uint8_t x = 3+6; x >= 3; x--) {
        matrix.drawFillTriangle(5+8, 3, matrix.getWidth()-2, 10+10, x, 8, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }

    for (uint8_t y = 10+10; y >= 10; y--) {
        matrix.drawFillTriangle(5+8, 3, matrix.getWidth()-2, y, 3, 8, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }

    for (uint8_t x = 5+8; x >= 5; x--) {
        matrix.drawFillTriangle(x, 3, matrix.getWidth()-2, 10, 3, 8, 1);
        matrix.display();
        delay(100);
        matrix.clear();
    }
}

/**************************************************************************/
/*!
  @brief    An example of the string drawer.
*/
/**************************************************************************/
void demoStrings() {
    matrix.drawString(0, 0, "test", 4, 1);
    matrix.drawString(0, 8, "TEST", 4, 1);
    matrix.drawString(0, 16, "[!+]", 4, 1);
    matrix.display();
    delay(2000);
    matrix.clear();

    matrix.setFont(FONT_4X6);
    
    matrix.drawString(0, 0, "test", 4, 1);
    matrix.drawString(0, 8, "TEST", 4, 1);
    matrix.drawString(0, 16, "[!+]", 4, 1);
    matrix.display();
    delay(2000);
    matrix.clear();

    matrix.setFont(FONT_5X7);
    
    matrix.drawString(0, 0, "test", 4, 1);
    matrix.drawString(0, 8, "TEST", 4, 1);
    matrix.drawString(0, 16, "[!+]", 4, 1);
    matrix.display();
    delay(2000);
    matrix.clear();

    matrix.setFont(FONT_3X5);
}
