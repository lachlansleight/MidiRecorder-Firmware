#include "Button.h"
#include <ButtonDebounce.h>

Button::Button(int pin, unsigned long debounceTime, void (*callback)(int)) : handler(pin, debounceTime)
{
    handler.setCallback(callback);
    reset();
}

void Button::update(int delta)
{
    down = false;
    up = false;
    handler.update();
    time = up ? time : (is ? time + delta : 0);
}

void Button::buttonChanged(int state)
{
    if (state != 0) {
        is = true;
        down = true;
    } else {
        if(is) up = true;
        is = false;
    }
}

void Button::reset()
{
    is = false;
    down = false;
    up = false;
    time = 0;
}