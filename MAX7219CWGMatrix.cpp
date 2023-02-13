/*
 * File:      MAX7219CWGMatrix.h
 * Authors:   Luke de Munk
 * Class:     MAX7219CWGMatrix
 * 
 * Library for a MAX7219CWG based 8x8 matrix. Can be used
 * to control multiple 8x8 matrices. For more info, checkout:
 * https://github.com/LukedeMunk/ESP32-8x8ledmatrix-big-display
 */
#include "MAX7219CWGMatrix.h"
#include <Arduino.h>

/**************************************************************************/
/*!
  @brief    Constructor.
  @param    numSegmentsHorizontal   Number of horizontal segments
  @param    numSegmentsVertical     Number of vertical segments
  @param    csPin                   Chip-Select pin
  @param    wiringType              Type of circuit is used. Check circuit diagram.
*/
/**************************************************************************/
MAX7219CWGMatrix::MAX7219CWGMatrix(uint8_t numSegmentsHorizontal, uint8_t numSegmentsVertical, uint8_t csPin, uint8_t wiringType) {
    _numSegmentsHorizontal = numSegmentsHorizontal;
    _numSegmentsVertical = numSegmentsVertical;
    _numSegments = _numSegmentsHorizontal * _numSegmentsVertical;

    _csPin = csPin;
    _wiringType = wiringType;
    _rotation = STANDARD_ROTATION;
    _width = _numSegmentsHorizontal * ROW_SIZE;
    _height = _numSegmentsVertical * COLUMN_SIZE;

    _font = 0;
    _fontRows = 0;
    _fontCols = 0;

    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, 1);

    SPI.begin();

    clear();
    display();
    setFont(FONT_3X5);
    setPower(false);
    setIntensity(0);

    _sendCommand(OPCODE_TEST | 0);                                          //Disable test mode
    _sendCommand(OPCODE_DECODE | 0);                                        //Disable decode mode
    _sendCommand(OPCODE_SCAN_LIMIT | 7);                                    //Display all lines
}

/**************************************************************************/
/*!
  @brief    Sets the power of the display.
  @param    on              Turn on (true), Turn off (false)
*/
/**************************************************************************/
void MAX7219CWGMatrix::setPower(bool on) {
	_sendCommand(OPCODE_ENABLE | (on ? 1: 0));
}

/**************************************************************************/
/*!
  @brief    Sets the intensity of the display.
  @param    level           Level of intensity (0-15)
*/
/**************************************************************************/
void MAX7219CWGMatrix::setIntensity(uint8_t level) {
	/* Check maximum intensity */
	if (level > 0xF) {
        level = 0xF;
    }	
	_sendCommand(OPCODE_INTENSITY | level);
}

/**************************************************************************/
/*!
  @brief    Sets the rotation of the display.
  @param    rotation        Rotation of the display
*/
/**************************************************************************/
void MAX7219CWGMatrix::setRotation(uint8_t rotation) {
    if (rotation != STANDARD_ROTATION && rotation != UPSIDE_DOWN_ROTATION) {
        debugln("ERROR: Invalid rotation given. Ignoring it.");
        return;
    }
	_rotation = rotation;
}

/**************************************************************************/
/*!
  @brief    Sets the font.
  @param    font            Number of the font.
*/
/**************************************************************************/
void MAX7219CWGMatrix::setFont(uint8_t font) {
    _font = font;
    if (_font == FONT_3X5) {
        _fontRows = FONT_3X5_ROWS;
        _fontCols = FONT_3X5_COLS;
    } else if (_font == FONT_4X6) {
        _fontRows = FONT_4X6_ROWS;
        _fontCols = FONT_4X6_COLS;
    } else if (_font == FONT_5X7) {
        _fontRows = FONT_5X7_ROWS;
        _fontCols = FONT_5X7_COLS;
    } else {
        _font = FONT_3X5;
        _fontRows = FONT_3X5_ROWS;
        _fontCols = FONT_3X5_COLS;
    }
}

/**************************************************************************/
/*!
  @brief    Draws a pixel.
  @param    x               X coordinate of the pixel
  @param    y               Y coordinate of the pixel
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawPixel(uint8_t x, uint8_t y, uint8_t value) {
    /* Check is coordinates are on display */
    if (x >= _width || y >= _height) {
        return;
    }

    /* If rotation is upside down, invert x and y*/
    if (_rotation == UPSIDE_DOWN_ROTATION) {
        x = _numSegmentsHorizontal*ROW_SIZE-1 - x;
        y = _numSegmentsVertical*COLUMN_SIZE-1 - y;
    }

    uint8_t segment = x/COLUMN_SIZE;                                        //Select segment
	uint16_t b = 7 - (x & 7);		                                        //Extract bit

    /* (For now,) if value != 0, turn led on else turn off */
	if (value) {
		_matrix[segment][y] |= (1<<b);
    }	else {
		_matrix[segment][y] &= ~(1<<b);
    }
}

/**************************************************************************/
/*!
  @brief    Draws a line with Bresenham's algorithm.
  @param    x0              Start point x coordinate
  @param    y0              Start point y coordinate
  @param    x1              End point x coordinate
  @param    y1              End point y coordinate
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t value) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep) {
        _swap_byte(x0, y0);
        _swap_byte(x1, y1);
    }
  
    if (x0 > x1) {
        _swap_byte(x0, x1);
        _swap_byte(y0, y1);
    }
  
    int8_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    
    int16_t err = dx / 2;
    int8_t ystep;
    
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    
    for (; x0 <= x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, value);
        } else {
            drawPixel(x0, y0, value);
        }

        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/**************************************************************************/
/*!
  @brief    Draws a line with an angle.
  @param    x0              Start point x coordinate
  @param    y0              Start point y coordinate
  @param    l               Length of the line
  @param    angle           Angle of the line (degrees)
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawLineAngle(uint8_t x0, uint8_t y0, uint8_t l, uint16_t angle, uint8_t value) {
    if (angle > 360) {
        angle = angle % 360;
    }
    
    uint8_t x = x0 + l * sin(angle * 0.0174532925);
    uint8_t y = y0 + l * cos(angle * 0.0174532925);

    drawLine(x0, y0, x, y, value);
}

/**************************************************************************/
/*!
  @brief    Draws a vertical line.
  @param    x               Start x coordinate
  @param    y               Start y coordinate
  @param    h               Height in pixels
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t value) {
    if (h == 0) {
        h = 1;
    }
    
    drawLine(x, y, x, y+h-1, value);
}

/**************************************************************************/
/*!
  @brief    Draws a horizontal line.
  @param    x               Start x coordinate
  @param    y               Start y coordinate
  @param    w               Width in pixels
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t value) {
    if (w == 0) {
        w = 1;
    }

    drawLine(x, y, x+w-1, y, value);
}

/**************************************************************************/
/*!
  @brief    Draws a rectangle with no fill.
  @param    x               Top left corner x coordinate
  @param    y               Top left corner y coordinate
  @param    w               Width in pixels
  @param    h               Height in pixels
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t value) {
    drawHLine(x, y, w, value);
    drawHLine(x, y+h-1, w, value);
    drawVLine(x, y, h, value);
    drawVLine(x+w-1, y, h, value);
}

/**************************************************************************/
/*!
  @brief    Fills a rectangle completely with one value.
  @param    x               Top left corner x coordinate
  @param    y               Top left corner y coordinate
  @param    w               Width in pixels
  @param    h               Height in pixels
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawFillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t value) {
    for (uint8_t i = x; i < x+w; i++) {
        drawVLine(i, y, h, value);
    }
}

/**************************************************************************/
/*!
  @brief    Draws a circle outline.
  @param    x0              Center-point x coordinate
  @param    y0              Center-point y coordinate
  @param    r               Radius of circle
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawCircle(uint8_t x0, uint8_t y0, int16_t r, uint8_t value) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    drawPixel(x0, y0+r, value);
    drawPixel(x0, y0-r, value);
    drawPixel(x0+r, y0, value);
    drawPixel(x0-r, y0, value);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        drawPixel(x0 + x, y0 + y, value);
        drawPixel(x0 - x, y0 + y, value);
        drawPixel(x0 + x, y0 - y, value);
        drawPixel(x0 - x, y0 - y, value);
        drawPixel(x0 + y, y0 + x, value);
        drawPixel(x0 - y, y0 + x, value);
        drawPixel(x0 + y, y0 - x, value);
        drawPixel(x0 - y, y0 - x, value);
    }
}

/**************************************************************************/
/*!
  @brief    Draw a circle with filled value.
  @param    x0              Center-point x coordinate
  @param    y0              Center-point y coordinate
  @param    r               Radius of circle
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawFillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t value) {
    drawVLine(x0, y0-r, 2*r+1, value);
    _fillCircleHelper(x0, y0, r, 3, 0, value);
}

/**************************************************************************/
/*!
  @brief    Draw a triangle with no fill value.
  @param    x0              Vertex #0 x coordinate
  @param    y0              Vertex #0 y coordinate
  @param    x1              Vertex #1 x coordinate
  @param    y1              Vertex #1 y coordinate
  @param    x2              Vertex #2 x coordinate
  @param    y2              Vertex #2 y coordinate
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value) {  
    drawLine(x0, y0, x1, y1, value);
    drawLine(x1, y1, x2, y2, value);
    drawLine(x2, y2, x0, y0, value);
}

/**************************************************************************/
/*!
  @brief    Draw a triangle with value-fill.
  @param    x0              Vertex #0 x coordinate
  @param    y0              Vertex #0 y coordinate
  @param    x1              Vertex #1 x coordinate
  @param    y1              Vertex #1 y coordinate
  @param    x2              Vertex #2 x coordinate
  @param    y2              Vertex #2 y coordinate
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawFillTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value) {
    int16_t a, b, y, last;
    
    /* Sort coordinates by Y order (y2 >= y1 >= y0) */
    if (y0 > y1) {
        _swap_byte(y0, y1); _swap_byte(x0, x1);
    }
    if (y1 > y2) {
        _swap_byte(y2, y1); _swap_byte(x2, x1);
    }
    if (y0 > y1) {
        _swap_byte(y0, y1); _swap_byte(x0, x1);
    }

    /* Handle awkward all-on-same-line case as its own thing */
    if(y0 == y2) { 
        a = b = x0;

        if (x1 < a){ 
            a = x1;
        } else if (x1 > b) {
            b = x1;
        }

        if (x2 < a) {
            a = x2;
        } else if (x2 > b) {
            b = x2;
        }

        drawHLine(a, y0, b-a+1, value);
        return;
    }
  
    int16_t dx01 = x1 - x0;
    int16_t dy01 = y1 - y0;
    int16_t dx02 = x2 - x0;
    int16_t dy02 = y2 - y0;
    int16_t dx12 = x2 - x1;
    int16_t dy12 = y2 - y1;

    int32_t sa = 0;
    int32_t sb = 0;
  
    /* For upper part of triangle, find scanline crossings for segments
    * 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    * is included here (and second loop will be skipped, avoiding a /0
    * error there), otherwise scanline y1 is skipped here and handled
    * in the second loop...which also avoids a /0 error here if y0=y1
    * (flat-topped triangle).
    */
    if (y1 == y2) {
        last = y1;                                                          //Include y1 scanline
    } else {
        last = y1 - 1;                                                      //Skip it
    }
  
    for(y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;

        /* longhand:
         * a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
         * b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if (a > b) {
            _swap_byte(a, b);
        }
        drawHLine(a, y, b-a+1, value);
    }
    
    /* For lower part of triangle, find scanline crossings for segments
     * 0-2 and 1-2.  This loop is skipped if y1=y2.
     */
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);

    for(; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;

        /* longhand:
         * a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
         * b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if (a > b) {
            _swap_byte(a, b);
        }
        drawHLine(a, y, b-a+1, value);
    }
}

/**************************************************************************/
/*!
  @brief    Draws a string.
  @param    x               X coordinate 
  @param    y               Y coordinate 
  @param    string          String to be drawn
  @param    length          Number of characters in the string
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::drawString(uint8_t x, uint8_t y, char string[], uint8_t length, uint8_t value) {
    for (int character = 0; character < length; character++) {
        _drawChar(x+character+(character*_fontCols), y, string[character], value);
    }
}

/**************************************************************************/
/*!
  @brief    Returns the value of a pixel.
  @param    x               X coordinate of the pixel
  @param    y               Y coordinate of the pixel
  @returns  value           Value of the pixel (for now 0 or 1)
*/
/**************************************************************************/
uint8_t MAX7219CWGMatrix::getPixel(uint8_t x, uint8_t y) {
    if (x > _numSegmentsHorizontal*COLUMN_SIZE || y > _numSegmentsVertical*ROW_SIZE) {
        debugln("ERROR: Invalid x or y coordinate given while asking value.");
        return 0;
    }

	uint16_t b = 7 - (x & 7);

	return _matrix[x][y] & (1 << b);                                        //Return extracted bit
}

/**************************************************************************/
/*!
  @brief    Returns the width in pixels.
  @returns  _width          Width in pixels
*/
/**************************************************************************/
uint8_t MAX7219CWGMatrix::getWidth() {
    return _width;
}

/**************************************************************************/
/*!
  @brief    Returns the height in pixels.
  @returns  _height         Height in pixels
*/
/**************************************************************************/
uint8_t MAX7219CWGMatrix::getHeight() {
    return _height;
}

/**************************************************************************/
/*!
  @brief    Shoots the display buffer in the display. Calculates order
            depending on the wiring type.
*/
/**************************************************************************/
void MAX7219CWGMatrix::display() {
    /* Currently only zigzag wiring supported */
    if (_wiringType != ZIGZAG_WIRING) {
        debugln("ERROR: Wiringtype not supported, choose 'ZIGZAG_WIRING'");
        return;
    }

    uint8_t rowAddress;
    uint8_t matrixRow;
    int16_t r2;
    
    for (int16_t r = 0; r < ROW_SIZE; r++){
        rowAddress = r;
        if (r >= 2*ROW_SIZE) {
            rowAddress = r - 2*ROW_SIZE;
        } else if (r >= ROW_SIZE) {
            rowAddress = r - ROW_SIZE;
        }
      
        SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
        digitalWrite(_csPin, 0);

        for (uint8_t segRow = 0; segRow < _numSegmentsVertical; segRow++) {
            r2 = r + segRow*ROW_SIZE;

            if (segRow % 2 == 1) {
                matrixRow = rowAddress;

                for (int16_t d = _numSegmentsHorizontal; d != -1; d--) {
                    uint8_t data = _matrix[d][r2];
                    _reverse(data);
                    uint16_t cmd = ((matrixRow + 1) << 8) | data;
                    SPI.transfer16(cmd);
                }
            } else {
                matrixRow = 7-rowAddress;

                for (int16_t d = 0; d != _numSegmentsHorizontal; d++) {
                    uint16_t cmd = ((matrixRow + 1) << 8) | _matrix[d][r2];
                    SPI.transfer16(cmd);
                }
            }
        }
        digitalWrite(_csPin, 1);
        SPI.endTransaction();
    }
}

/**************************************************************************/
/*!
  @brief    Clears display buffer.
*/
/**************************************************************************/
void MAX7219CWGMatrix::clear() {
    for (uint8_t y = 0; y < _numSegmentsVertical*ROW_SIZE; y++) {
        for (uint8_t x = 0; x < _numSegmentsHorizontal; x++) {
            _matrix[x][y] = 0;
        }
    }
}

/**************************************************************************/
/*!
  @brief    Sends a command to the displays.
  @param    command         Command do execute (see datasheet)
*/
/**************************************************************************/
void MAX7219CWGMatrix::_sendCommand(uint16_t command) {
	SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, 0);

	/* Send the same command to all segments */
	for (uint8_t i = 0; i < _numSegments; ++i)	{
		SPI.transfer16(command);
	}
    
	digitalWrite(_csPin, 1);
	SPI.endTransaction();
}

/**************************************************************************/
/*!
  @brief    A helper function used to reverse bits in a byte.
  @param    b           Pointer to an 8-bit unsigned integer
*/
/**************************************************************************/
void MAX7219CWGMatrix::_reverse(uint8_t& b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
}

/**************************************************************************/
/*!
  @brief    Quarter-circle drawer with fill, used for circles.
  @param    x0          Center-point x coordinate
  @param    y0          Center-point y coordinate
  @param    r           Radius of circle
  @param    corners     Mask bits indicating which quarters we're doing
  @param    delta       Offset from center-point, used for round-rects
  @param    value       Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::_fillCircleHelper(uint8_t x0, uint8_t y0, int16_t r, uint8_t corners, int16_t delta, uint8_t value) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;
    
    delta++;                                                                // Avoid some +1's in the loop
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;

        /* These checks avoid double-drawing certain lines, important
         * for the SSD1306 library which has an INVERT drawing mode.
         */
        if (x < (y + 1)) {
            if (corners & 1) {
                drawVLine(x0+x, y0-y, 2*y+delta, value);
            }
            if (corners & 2) {
                drawVLine(x0-x, y0-y, 2*y+delta, value);
            }
        }

        if (y != py) {
            if (corners & 1) {
                drawVLine(x0+py, y0-px, 2*px+delta, value);
            }
            if (corners & 2) {
                drawVLine(x0-py, y0-px, 2*px+delta, value);
            }

            py = y;
        }
        px = x;
    }
}

/**************************************************************************/
/*!
  @brief    Draws a character.
  @param    x           x coordinate of most left column of leds
  @param    y           y coordinate of lowest row of leds
  @param    character   Character to be drawn
  @param    value       Value to fill (0-1)
*/
/**************************************************************************/
void MAX7219CWGMatrix::_drawChar(uint8_t x, uint8_t y, char character, uint8_t value) {
    uint8_t i = 0;
    uint8_t bitCounter = 0b00000001;

    if (_font == FONT_3X5) {
        while (i < FONT_3X5_SIZE) {
            if (character == FontToIndex3x5[i]) {
                break;
            }
            i++;
        }
    } else if (_font == FONT_4X6) {
        while (i < FONT_4X6_SIZE) {
            if (character == FontToIndex4x6[i]) {
                break;
            }
            i++;
        }
    } else if (_font == FONT_5X7) {
        while (i < FONT_5X7_SIZE) {
            if (character == FontToIndex5x7[i]) {
                break;
            }
            i++;
        }
    }
    
    uint16_t memAddress = i*_fontCols;
    uint8_t columnValue = 0;
        
    for (uint8_t column = 0; column < _fontCols; column++) {
        for (uint8_t row = _fontRows; row > 0; row--) {
            if (_font == FONT_3X5) {
                columnValue = pgm_read_byte_near(Font3x5 + (memAddress+column));  
            } else if (_font == FONT_4X6) {
                columnValue = pgm_read_byte_near(Font4x6 + (memAddress+column));
            } else if (_font == FONT_5X7) {
                columnValue = pgm_read_byte_near(Font5x7 + (memAddress+column));
            }

            if (columnValue & bitCounter) {
                drawPixel(x+column, y+row-1, value);
            }
            bitCounter = bitCounter << 1;
        }
        bitCounter = 0b00000001;
    }
}
