#include "Flags.h"
#include "InputHandling.h"
#include "Connectivity.h"
#include "LedHandling.h"
#include "MidiHandling.h"
#include "SongHandling.h"

int deltaMillis = 0;
long lastMillis;

void setup() {
    //allocate this memory first before anything else can break into my beautiful, pristine chunk
    allocateRecordingMemory();

    Serial.begin(38400);
    delay(1000);
    
    setupLed();
    setupNetwork();
    setupMidi();
}

void loop() {
    deltaMillis = int(millis() - lastMillis);
    lastMillis = millis();
    ButtonUpload.update(deltaMillis);

    //song auto-ends if there's been a certain amount of time passed with no new messages
    //todo - this should probably suspend if there are notes held. Maybe.
    if(runningSong) {
        songTime = (unsigned long)(millis() - songStartTime);
        if(songTime - lastNoteTime > timeoutTime || ButtonUpload.down) {
          endSong();
        }
    }
    readMidi();
}