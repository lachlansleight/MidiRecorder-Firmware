#ifndef SONGHANDLING_H
#define SONGHANDLING_H

#include <Arduino.h>

struct Message {
    byte onOffPitch;
    byte isPedalVelocity;
    byte timeA;
    byte timeB;
    byte timeFrac;
};

void allocateRecordingMemory();
void startSong();
void endSong();
void addMessage(bool onOff, byte pitch, byte velocity, bool isPedal);
void printMessage(Message message);

extern bool runningSong;
extern unsigned long songTime;
extern unsigned long songStartTime;
extern unsigned long lastNoteTime;
extern long timeoutTime;

#endif