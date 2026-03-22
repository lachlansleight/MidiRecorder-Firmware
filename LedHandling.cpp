#include "LedHandling.h"

void setupLed() {
    ledcSetup(0, 5000, 8);
    ledcSetup(1, 5000, 8);
    ledcSetup(2, 5000, 8);
    
    ledcAttachPin(PIN_LED_R, 0);
    ledcAttachPin(PIN_LED_G, 1);
    ledcAttachPin(PIN_LED_B, 2);
}

void setLedColor(byte r, byte g, byte b) {
    ledcWrite(0, r);
    ledcWrite(1, g);
    ledcWrite(2, b);
}

void errorFlash()
{
    //flash LED white/red, then fade
    
    byte last = 255;
    for(byte i = 255; i > 0; i--) {
        if(i > last) break;
        last = i;
        
        setLedColor(255, i, i);
        delay(1);
    }
    last = 0;
    for(byte i = 0; i < 255; i++) {
        if(i < last) break;
        last = i;
        
        setLedColor(255, i, i);
        delay(1);
    }
    last = 255;
    for(byte i = 255; i > 0; i--) {
        if(i > last) break;
        last = i;
        
        setLedColor(255, i, i);
        delay(1);
    }
    delay(500);
    last = 255;
    for(byte i = 255; i > 0; i--) {
        if(i > last) break;
        last = i;
        
        setLedColor(i, 0, 0);
        delay(5);
    }

    setLedColor(0, 0, 0);
}

void wifiSuccessFlash()
{
    byte last = 255;
    for(byte i = 255; i > 0; i--) {
        if(i > last) break;
        last = i;
        
        setLedColor(i, i * 0.5 + 128, 255);
        delay(1);
    }
    last = 255;
    for(byte i = 255; i > 0; i--) {
        if(i > last) break;
        last = i;
        
        setLedColor(0, i * 0.5, i);
        delay(5);
    }

    setLedColor(0, 0, 0);
}