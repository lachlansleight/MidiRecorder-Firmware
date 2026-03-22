#ifndef MIDIHANDLING_H
#define MIDIHANDLING_H

#include <Arduino.h>
#include <MIDI.h>

void setupMidi();
void readMidi();
void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte control, byte value);

#endif