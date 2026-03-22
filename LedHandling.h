#ifndef LEDHANDLING_H
#define LEDHANDLING_H

#include <Arduino.h>

#define PIN_LED_R 25
#define PIN_LED_G 24
#define PIN_LED_B 23

void setupLed();
void setLedColor(byte r, byte g, byte b);
void errorFlash();
void wifiSuccessFlash();

#endif