/*
 * File:      SmartLedDisplay.h
 * Authors:   Luke de Munk
 * Class:     SmartLedDisplay
 * 
 * This SmartLedDisplay class can be used to make a custom led matrix
 * display. For more info, checkout:
 * https://github.com/LukedeMunk/ESP32-8x8ledmatrix-big-display
 */
#include "SmartLedDisplay.h"

/**************************************************************************/
/*!
  @brief    Constructor.
  @param    numSegmentsHorizontal   Number of horizontal segments
  @param    numSegmentsVertical     Number of vertical segments
  @param    csPin                   Chip-Select pin
  @param    wiringType              Type of circuit is used. Check circuit diagram.
*/
/**************************************************************************/
SmartLedDisplay::SmartLedDisplay(uint8_t numSegmentsHorizontal, uint8_t numSegmentsVertical, uint8_t csPin, uint8_t wiringType) {
    _matrix.initialiseMatrix(numSegmentsHorizontal, numSegmentsVertical, csPin, wiringType);

    /* Initialise the display */
    _matrix.setPower(true);
    _matrix.setIntensity(0);
    _matrix.setRotation(UPSIDE_DOWN_ROTATION);
    _matrix.display();
    
    _time.minute = 0;
    _time.hour = 0;

    _months[0] = JANUARY;
    _months[1] = FEBRUARY;
    _months[2] = MARCH;
    _months[3] = APRIL;
    _months[4] = MAY;
    _months[5] = JUNE;
    _months[6] = JULY;
    _months[7] = AUGUST;
    _months[8] = SEPTEMBER;
    _months[9] = OCTOBER;
    _months[10] = NOVEMBER;
    _months[11] = DECEMBER;

    _date.day = 0;
    _date.month = 0;
    _date.year = 0;

    _longDate.dayName = MONDAY;
    _longDate.day = 0;
    _longDate.month = 0;
}

/**************************************************************************/
/*!
  @brief    Sets the power of the display.
  @param    on              Turn on (true), Turn off (false)
*/
/**************************************************************************/
void SmartLedDisplay::setPower(bool on) {
	_matrix.setPower(on);
}

/**************************************************************************/
/*!
  @brief    Sets the intensity of the display.
  @param    level           Level of intensity (0-15)
*/
/**************************************************************************/
void SmartLedDisplay::setIntensity(uint8_t level) {
	_matrix.setIntensity(level);
}

/**************************************************************************/
/*!
  @brief    Sets the rotation of the display.
  @param    rotation        Rotation of the display
*/
/**************************************************************************/
void SmartLedDisplay::setRotation(uint8_t rotation) {
    _matrix.setRotation(rotation);
}

/**************************************************************************/
/*!
  @brief    Sets the if the display is inverted. (0 becomes 1 and vice versa)
  @param    inverted        True if the display is inverted.
*/
/**************************************************************************/
void SmartLedDisplay::setInverted(bool inverted) {
    _matrix.setInverted(inverted);
}

/**************************************************************************/
/*!
  @brief    Sets time to display.
  @param    time            Time object to describe the time
*/
/**************************************************************************/
void SmartLedDisplay::setTime(Time time) {
    _time.second = time.second;
    _time.minute = time.minute;
    _time.hour = time.hour;
}

/**************************************************************************/
/*!
  @brief    Scrolls a string.
  @param    x               X coordinate of leftest column of leds
  @param    y               Y coordinate of lowest row of leds
  @param    width           Maximum width in pixels
  @param    string          String to be shown
  @param    length          Length of the string (number of characters)
  @param    value           Value to fill (0-1)
  @param    scrollDelay     Delay per timestep in ms
*/
/**************************************************************************/
void SmartLedDisplay::showScrollingString(uint8_t x, uint8_t y, uint8_t width, char string[], uint8_t length, uint8_t value, uint8_t scrollDelay) {
    int spacing = length*_matrix.getFontCols();
    
    for (int cursor = width-1; cursor > 0-spacing; cursor--) {
        for (int character = 0; character < length; character++) {
            if (x+character+(cursor+character*_matrix.getFontCols()) < x + width && x+character+(cursor+character*_matrix.getFontCols()) > x) {
                _matrix.drawChar(x+character+(cursor+character*_matrix.getFontCols()), y, string[character], value);
            }
        }
        display();
        delay(scrollDelay);
        clear();
    }
}

/**************************************************************************/
/*!
  @brief    Prints time digitally.
  @param    x               X coordinate of leftest column of leds
  @param    y               Y coordinate of lowest row of leds
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void SmartLedDisplay::printDigitalTime(uint8_t x, uint8_t y, uint8_t value) {
    uint8_t length = 0;

    if (_time.second == 255) {
        length = 5;                                                         //Without +1 for line ending, without second
    } else {
        length = 8+1;                                                       //+1 for line ending, with second
    }

    char timeString[length];

    if (_time.second == 255) {
        sprintf(timeString, "%02d:%02d", _time.hour, _time.minute);         //Contruct string, without second
    } else {
        sprintf(timeString, "%02d:%02d:%02d", _time.hour, _time.minute, _time.second);   //Contruct string, with second
    }

    _matrix.drawString(x, y, timeString, length, 1);
}

/**************************************************************************/
/*!
  @brief    Draws analog clock with time.
  @param    x               X coordinate of leftest column of leds
  @param    y               Y coordinate of lowest row of leds
  @param    r               Radius of clock
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void SmartLedDisplay::drawAnalogTime(uint8_t x, uint8_t y, uint8_t r, uint8_t value) {
    if (r < 5) {
        r = 5;
    }
    
    if (_time.second != 255) {
        uint16_t angleSecond = 360/60*_time.second;
        _matrix.drawLineAngle(x, y, r-1, angleSecond, value);
    }
    
    uint16_t angleMinute = 360/60*_time.minute;
    _matrix.drawLineAngle(x, y, r-2, angleMinute, value);
    
    uint16_t angleHour = 360/12*_time.hour;
    _matrix.drawLineAngle(x, y, r-4, angleHour, value);

    _matrix.drawCircle(x, y, r, value);
}

/**************************************************************************/
/*!
  @brief    Prints date in notation DD-MM-YY.
  @param    x               X coordinate of leftest column of leds
  @param    y               Y coordinate of lowest row of leds
  @param    date            Date object
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void SmartLedDisplay::printShortDate(uint8_t x, uint8_t y, Date date, uint8_t value) {
    uint8_t length = 10+1;                                                  //+1 for line ending
    char dateString[length];

    sprintf(dateString, "%02d-%02d-%d", date.day, date.month, date.year);   //Contruct string

    _matrix.drawString(x, y, dateString, length, 1);
}

/**************************************************************************/
/*!
  @brief    Prints date in notation [day] [date] [month].
  @param    x               X coordinate of leftest column of leds
  @param    y               Y coordinate of lowest row of leds
  @param    date            Date object
  @param    value           Value to fill (0-1)
*/
/**************************************************************************/
void SmartLedDisplay::printLongDate(uint8_t x, uint8_t y, LongDate date, uint8_t value) {
    uint8_t length = date.dayName.length() + 2 + _months[date.month].length()+1;    //+1 for line ending
    char dateString[length];

    sprintf(dateString, "%s %d %s", date.dayName, date.day, _months[date.month]);   //Contruct string
    showScrollingString(x, y, _matrix.getWidth()-x, dateString, length, value);
    //drawString(x, y, dateString, length, 1);
}

/**************************************************************************/
/*!
  @brief    Shows screen 1 with current values.
*/
/**************************************************************************/
void SmartLedDisplay::showScreen1() {
    clear();
    _matrix.drawRectangle(0, 0, getWidth(), getHeight(), 1);
    drawAnalogTime(15, 15, 7, 1);
    _time.second = 255;                                                     //Reset second to write short digital clock
    printDigitalTime(6, 2, 1);
    display();
}

/**************************************************************************/
/*!
  @brief    Shows screen 2 with current values.
*/
/**************************************************************************/
void SmartLedDisplay::showScreen2() {
    clear();
    _matrix.drawString(0, 0, "Screen2", 7, 1);
    display();
}

/**************************************************************************/
/*!
  @brief    Shows screen 3 with current values.
*/
/**************************************************************************/
void SmartLedDisplay::showScreen3() {
    clear();
    _matrix.drawString(0, 0, "Screen3", 7, 1);
    display();
}

/**************************************************************************/
/*!
  @brief    Returns the width in pixels.
  @returns  width           Width in pixels
*/
/**************************************************************************/
uint8_t SmartLedDisplay::getWidth() {
    return _matrix.getWidth();
}

/**************************************************************************/
/*!
  @brief    Returns the height in pixels.
  @returns  height          Height in pixels
*/
/**************************************************************************/
uint8_t SmartLedDisplay::getHeight() {
    return _matrix.getHeight();
}

/**************************************************************************/
/*!
  @brief    Returns the power state.
  @returns  power           True if power is on  
*/
/**************************************************************************/
bool SmartLedDisplay::getPower() {
    return _matrix.getPower();
}

/**************************************************************************/
/*!
  @brief    Returns the intensity.
  @returns  intensity       0 = lowest, 15 = highest
*/
/**************************************************************************/
uint8_t SmartLedDisplay::getIntensity() {
    return _matrix.getIntensity();
}

/**************************************************************************/
/*!
  @brief    Returns if display is inverted.
  @returns  inverted        True if display is inverted.
*/
/**************************************************************************/
bool SmartLedDisplay::getInverted() {
    return _matrix.getInverted();
}

/**************************************************************************/
/*!
  @brief    Returns the time of the display.
  @returns  _time           Time object
*/
/**************************************************************************/
Time SmartLedDisplay::getTime() {
    return _time;
}

/**************************************************************************/
/*!
  @brief    Displays printed and drawed objects.
*/
/**************************************************************************/
void SmartLedDisplay::display() {
    _matrix.display();
}

/**************************************************************************/
/*!
  @brief    Clears the display.
*/
/**************************************************************************/
void SmartLedDisplay::clear() {
    _matrix.clear();
}
