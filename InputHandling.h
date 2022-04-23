#ifndef INPUTHANDLING_H
#define INPUTHANDLING_H

#include <Arduino.h>
#include <ButtonDebounce.h>
#include "Button.h"

#define PIN_BTN_UPLOAD 13
#define DEBOUNCE_TIME 50

extern Button ButtonUpload;
void buttonChangedUpload(int state);

#endif

