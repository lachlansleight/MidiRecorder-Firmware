#include "MidiHandling.h"
#include "SongHandling.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

void setupMidi() {
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.begin();
}

void readMidi() {
    MIDI.read();
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    if(channel != 1) return;
    //Serial.println("ON\t" + String(channel) + "\t" + String(pitch) + "\t" + String(velocity));
    addMessage(true, pitch, velocity, false);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    if(channel != 1) return;

    //Serial.println("OFF\t" + String(channel) + "\t" + String(pitch) + "\t" + String(velocity));
    addMessage(false, pitch, velocity, false);
}

void handleControlChange(byte channel, byte number, byte value)
{
    if(channel != 1) return;
    
    //Serial.println("CC\t" + String(channel) + "\t" + String(number) + "\t" + String(value));
    if(number == 64) {
        //dampen pedal
        if(value == 127) addMessage(true, 0, 0, true);
        else addMessage(true, 0, 0, false);
    } else if(number == 67) {
        //sost pedal - could use for some kind of input?
        //I'm thinking a hold of three seconds on sost should discard the current song without uploading
    } else {
        //unknown / uncared-about
    }
}