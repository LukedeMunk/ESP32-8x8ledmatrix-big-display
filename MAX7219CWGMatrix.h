/*
 * File:      MAX7219CWGMatrix.h
 * Authors:   Luke de Munk
 * Class:     MAX7219CWGMatrix
 * 
 * Library for a MAX7219CWG based 8x8 matrix. Can be used
 * to control multiple 8x8 matrices. For more info, checkout:
 * https://github.com/LukedeMunk/ESP32-8x8ledmatrix-big-display
 */
#ifndef MAX7219CWG_MATRIX_H
#define MAX7219CWG_MATRIX_H
#include <SPI.h>
#include <Arduino.h>
#include "Debugger.h"                                                       //For serial debugging
#include "Font3x5.h"
#include "Font4x6.h"
#include "Font5x7.h"

/* Pins */
#define CLOCK_OUT_PIN           18                                          //Use hardware SPI GPIO clock pin for your hardware
#define DATAIN_PIN              3                                           //Use hardware SPI GPIO clock pin for your hardware

#define ROW_SIZE                8
#define COLUMN_SIZE             8

#define MAX_HORIZONTAL_SEGMENTS 4
#define MAX_VERTICAL_SEGMENTS   4

/* Others */
#define MAX_INTENSITY           0xF                                         //The maximum intensity value that can be set for a LED array

/* Data Connection Types, depends on hardware */
#define ZIGZAG_WIRING           0                                           //See wiring diagram

/* Rotation types */
#define STANDARD_ROTATION       0
#define UPSIDE_DOWN_ROTATION    1

/* Op codes as defined in the datasheet */
#define OPCODE_ENABLE           0x0C00
#define OPCODE_TEST             0x0F00
#define OPCODE_INTENSITY        0x0A00
#define OPCODE_SCAN_LIMIT       0x0B00
#define OPCODE_DECODE           0x0900

/* Days */
#define MONDAY                  "Monday"
#define TUESDAY                 "Tuesday"
#define WEDNESDAY               "Wednesday"
#define THURSDAY                "Thursday"
#define FRIDAY                  "Friday"
#define SATURDAY                "Saturday"
#define SUNDAY                  "Sunday"

/* Months */
#define JANUARY                 "January"
#define FEBRUARY                "February"
#define MARCH                   "March"
#define APRIL                   "April"
#define MAY                     "May"
#define JUNE                    "June"
#define JULY                    "July"
#define AUGUST                  "August"
#define SEPTEMBER               "September"
#define OCTOBER                 "October"
#define NOVEMBER                "November"
#define DECEMBER                "December"

/* Function used to swap two bytes */
#ifndef _swap_byte
#define _swap_byte(a, b) { uint8_t t = a; a = b; b = t; }
#endif

class MAX7219CWGMatrix {
	public:
        MAX7219CWGMatrix(uint8_t numSegmentsHorizontal, uint8_t numSegmentsVertical, uint8_t csPin, uint8_t wiringType = ZIGZAG_WIRING);
        MAX7219CWGMatrix();

        void initialiseMatrix(uint8_t numSegmentsHorizontal, uint8_t numSegmentsVertical, uint8_t csPin, uint8_t wiringType = ZIGZAG_WIRING);
        
        /* Config functions */
        void setPower(bool on);
        void setIntensity(uint8_t level);
        void setRotation(uint8_t rotation);
        void setFont(uint8_t font);
        void setInverted(bool inverted);

        /* Draw functions*/
        void drawPixel(uint8_t x, uint8_t y, uint8_t value);
        void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t value);
        void drawLineAngle(uint8_t x0, uint8_t y0, uint8_t l, uint16_t angle, uint8_t value);
        void drawVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t value);
        void drawHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t value);
        void drawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t value);
        void drawFillRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t value);
        void drawCircle(uint8_t x0, uint8_t y0, int16_t r, uint8_t value);
        void drawFillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t value);
        void drawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value);
        void drawFillTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value);

        void drawChar(uint8_t x, uint8_t y, char character, uint8_t value);
        void drawString(uint8_t x, uint8_t y, char string[], uint8_t length, uint8_t value);

        /* Getters */
        uint8_t getPixel(uint8_t x, uint8_t y);
        uint8_t getWidth();
        uint8_t getHeight();
        uint8_t getFontCols();
        bool getPower();
        uint8_t getIntensity();
        bool getInverted();

        /* Display and clear functions */
        void display();
        void clear();
		
	private:
        void _sendCommand(uint16_t command);

        void _reverse(uint8_t& b);
        
        void _fillCircleHelper(uint8_t x0, uint8_t y0, int16_t r, uint8_t corners, int16_t delta, uint8_t value);

        uint8_t _width;
        uint8_t _height;

        uint8_t _numSegmentsHorizontal;
        uint8_t _numSegmentsVertical;
        uint8_t _numSegments;
        uint8_t _wiringType;
        uint8_t _csPin;
        uint8_t _rotation;

        uint8_t _font;
        uint8_t _fontRows;
        uint8_t _fontCols;

        bool _power;
        uint8_t _intensity;
        uint8_t _inverted;

        uint8_t _matrix[MAX_HORIZONTAL_SEGMENTS][MAX_VERTICAL_SEGMENTS*ROW_SIZE];
};

#endif /* MAX7219CWG_MATRIX_H */
