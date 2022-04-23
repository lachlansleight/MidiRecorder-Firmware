#ifndef _BUTTON_H
#define _BUTTON_H

#include <Arduino.h>
#include <ButtonDebounce.h>

class Button {
    public:
        Button(int pin, unsigned long debounceTime, void (*callback)(int));
        ButtonDebounce handler;
        bool is;
        bool down;
        bool up;
        int time;
        void update(int delta);
        void buttonChanged(int state);
        void reset();
};

#endif