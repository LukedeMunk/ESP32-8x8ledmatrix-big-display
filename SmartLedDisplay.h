/*
 * File:      SmartLedDisplay.h
 * Authors:   Luke de Munk
 * Class:     SmartLedDisplay
 * 
 * This SmartLedDisplay class can be used to make a custom led matrix
 * display. For more info, checkout:
 * https://github.com/LukedeMunk/ESP32-8x8ledmatrix-big-display
 */
#ifndef SMART_LED_DISPLAY_H
#define SMART_LED_DISPLAY_H
#include "MAX7219CWGMatrix.h"
#include "Debugger.h"                                                       //For serial debugging

/* Days */
#define MONDAY          "Monday"
#define TUESDAY         "Tuesday"
#define WEDNESDAY       "Wednesday"
#define THURSDAY        "Thursday"
#define FRIDAY          "Friday"
#define SATURDAY        "Saturday"
#define SUNDAY          "Sunday"

/* Months */
#define JANUARY         "January"
#define FEBRUARY        "February"
#define MARCH           "March"
#define APRIL           "April"
#define MAY             "May"
#define JUNE            "June"
#define JULY            "July"
#define AUGUST          "August"
#define SEPTEMBER       "September"
#define OCTOBER         "October"
#define NOVEMBER        "November"
#define DECEMBER        "December"

struct Time {
    uint8_t hour;
    uint8_t minute;
    uint8_t second = 255;
};

struct Date {
    uint8_t day;
    uint8_t month;
    uint8_t year;
};

struct LongDate {
    String dayName;
    uint8_t day;
    uint8_t month;
};

class SmartLedDisplay {
	public:
        SmartLedDisplay(uint8_t numSegmentsHorizontal, uint8_t numSegmentsVertical, uint8_t csPin, uint8_t wiringType = ZIGZAG_WIRING);

        /* Config functions */
        void setPower(bool on);
        void setIntensity(uint8_t level);
        void setRotation(uint8_t rotation);
        void setInverted(bool inverted);
        void setTime(Time time);

        /* Draw functions*/
        void showScrollingString(uint8_t x, uint8_t y, uint8_t width, char string[], uint8_t length, uint8_t value, uint8_t scrollDelay = 100); //direction add to display class

        void printDigitalTime(uint8_t x, uint8_t y, uint8_t value);
        void drawAnalogTime(uint8_t x, uint8_t y, uint8_t r, uint8_t value);
        
        void printShortDate(uint8_t x, uint8_t y, Date date, uint8_t value);
        void printLongDate(uint8_t x, uint8_t y, LongDate date, uint8_t value);

        void drawBitmap();

        /* Screens */
        void showScreen1();
        void showScreen2();
        void showScreen3();

        /* Getters */
        uint8_t getWidth();
        uint8_t getHeight();
        bool getPower();
        uint8_t getIntensity();
        bool getInverted();
        Time getTime();
        
        /* Display and clear functions */
        void display();
        void clear();
		
	private:
        Time _time;
        String _months[12];
        Date _date;
        LongDate _longDate;
        
        MAX7219CWGMatrix _matrix;
};

#endif /* SMART_LED_DISPLAY_H */
