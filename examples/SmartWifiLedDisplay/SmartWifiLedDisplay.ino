/*
 * File:      SmartWifiLedDisplay.ino
 * Author:    Luke de Munk
 * 
 * Example file to test the SmartLedDisplay library with webcontrols.
 * For more info, checkout:
 * https://github.com/LukedeMunk/ESP32-8x8ledmatrix-big-display
 */
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "SmartLedDisplay.h"
#include "Debugger.h"                                                       //For serial debugging

#define SSID            "YOUR SSID"
#define PASSWORD        "YOUR PASSWORD"
AsyncWebServer server(80);                                                  //Create AsyncWebServer object on port 80

/* Define NTP Client to get time */
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

/* Pins */
#define CLOCK_OUT_PIN   18                                                  //Use hardware SPI GPIO clock pin for your hardware
#define DATAIN_PIN      23                                                  //Use hardware SPI GPIO data pin for your hardware
#define CS_PIN          14

#define WIDTH           4                                                   //4 segments horizontal
#define HEIGHT          3                                                   //3 segments vertical

SmartLedDisplay display(WIDTH, HEIGHT, CS_PIN);                             //Create a SmartLedDisplay object

uint8_t screen = 0;

/**************************************************************************/
/*!
  @brief    Replaces placeholders with actual data in HTML page.
*/
/**************************************************************************/
String processor(const String& var){
    if (var == "POWER") {
        return (String) display.getPower();
    } else if (var == "INTENSITY") {
        return (String) display.getIntensity();
    } else if (var == "SCREEN") {
        return (String) screen;
    } else if (var == "INVERTED") {
        return (String) display.getInverted();
    } else {
        return " placeholder_error ";
    }
    return String();
}

/**************************************************************************/
/*!
  @brief    Setup the controller.
*/
/**************************************************************************/
void setup() {
    Serial.begin(115200);                                                   //Serial port for debugging purposes
    
    /* Initialize SPIFFS */
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
        
    /* Connect to Wi-Fi */
    WiFi.begin(SSID, PASSWORD);
    debug("Connecting to WiFi.");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        debug(".");
    }
    
    debugln("");
    debug("IP: ");
    debugln(WiFi.localIP());

    timeClient.begin();                                                     //Initialize a NTPClient to get time
    timeClient.setTimeOffset(3600);                                         //GMT +2 = 7200 (for summer time), GMT +1 = 3600 (for winter time)

    /*
    *  Routes for loading all the necessary files
    */
    /* Load index.html file */
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    
    /* Load style.css file */
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });

    /* Load style_mobile.css file */
    server.on("/style_mobile.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style_mobile.css", "text/css");
    });

    /* Load style_switches.css file */
    server.on("/style_switches.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style_switches.css", "text/css");
    });

    /* Load favicon.ico file */
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/favicon.ico", "image/x-icon");
    });

    /* Load jquery.min.js file */
    server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/jquery.min.js", "text/script");
    });

    /* Load switches.js file */
    server.on("/switches.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/switches.js", "text/script");
    });

    /* Load base.js file */
    server.on("/base.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/base.js", "text/script");
    });
    /*
    * End of file loading
    */

    /*
    * Routes for receiving data
    */
    /* Route for power */
    server.on("/set_power", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("power")) {
            bool power = (bool) atoi(request->getParam("power")->value().c_str());
            display.setPower(power);
        }
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    /* Route for updating intensity */
    server.on("/set_intensity", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("intensity")) {
            uint8_t intensity = (uint8_t) atoi(request->getParam("intensity")->value().c_str());
            display.setIntensity(intensity);
        }
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    /* Route for updating screen */
    server.on("/set_screen", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("screen")) {
            screen = (uint8_t) atoi(request->getParam("screen")->value().c_str());
        }
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    /* Route for updating */
    server.on("/set_invert", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("inverted")) {
            bool inverted = (bool) atoi(request->getParam("inverted")->value().c_str());
            display.setInverted(inverted);
        }
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    /*
    * End of data receiving
    */

    server.begin();                                                         //Start server
}


/**************************************************************************/
/*!
  @brief    Mainloop.
*/
/**************************************************************************/
void loop() {
    updateTime();
    
    switch (screen) {
    case 0:
        display.showScreen1();
        break;

    case 1:
        display.showScreen2();
        break;

    case 2:
        display.showScreen3();
        break;
    
    default:
        break;
    }
    delay(100);
}

/**************************************************************************/
/*!
  @brief    Visits the UDP server and updates the time of the display.
*/
/**************************************************************************/
void updateTime() {
    Time t;
    
    while (!timeClient.update()) {
        timeClient.forceUpdate();
    }
    
    String formattedTime = timeClient.getFormattedTime();

    t.hour = atoi(formattedTime.substring(0, 2).c_str());
    t.minute = atoi(formattedTime.substring(3, 5).c_str());
    t.second = atoi(formattedTime.substring(6, 8).c_str());

    display.setTime(t);
}
