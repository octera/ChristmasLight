#define FASTLED_FORCE_SOFTWARE_PINS

#include <Arduino.h>
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ArduinoOTA.h>
#include <FastLED.h>
#include <LEDEffect.h>
#include <Ticker.h>
#include "effects/effects.hpp"

void shiftright (BaseEffect* myarray[], int size)
{
  BaseEffect* temp = myarray[0];
  for (int i=0; i<(size - 1); i++) {
    myarray[i] = myarray[i+1] ;
  }
  myarray[size - 1] = temp;
}

void wifiManager() {
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}

#define STR(x)  x
void arduinoOta() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  #ifdef BOARD_NAME
  ArduinoOTA.setHostname(STR(BOARD_NAME));
  #endif
  // No authentication by default
  ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

// Strip
#define DATA_PIN  D7  // CHANGEME
#ifndef NUM_LEDS
  #define NUM_LEDS  300  // CHANGEME
#endif

CRGB leds[NUM_LEDS];
BaseEffect* effects[] = {
  new MeteorRain("MeteorRain"),
  new BoucingBall("BouncingBall"),
  new RunningLights("RunningLight"),
  new FireEffect<NUM_LEDS>("fire"),
  new RainbowEffect("rainbow"),
  new TwinkleEffect<NUM_LEDS>("twinkle"),
  new ApplauseEffect("applause"),
  new JuggleEffect("juggle")
};
const uint8_t effectCount = 8;
LedEffect strip(effects, effectCount);
void setupLedEffect() {
    strip.begin(&FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS));
}


//Ticker part
#define DELAY_BETWEEN_EFFECT 120
Ticker effectChanger;
bool changeEffect = true;
void changeStateTicker() {
  effectChanger.attach(DELAY_BETWEEN_EFFECT, changeStateTicker);
  if (changeEffect) {
    shiftright(effects,effectCount);
    strip = LedEffect(effects, effectCount);
    strip.begin(&FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS));
  }
}
void ticker(){
  effectChanger.attach(DELAY_BETWEEN_EFFECT, changeStateTicker);

}

void setup() {
    // put your setup code here, to run once:
	  Serial.begin(9600);
	  Serial.println("resetting");

    wifiManager();
    arduinoOta();
    setupLedEffect();
    ticker();
}

void loop() {
  ArduinoOTA.handle();

  // Strip
  strip.loop();
}