#include "SongHandling.h"
#include "Flags.h"
#include "Connectivity.h"
#include "LedHandling.h"

unsigned long songStartTime = 0;
unsigned long songTime = 0;
unsigned long lastNoteTime = 0;
bool runningSong = false;

uint8_t* messageBuffer;
unsigned int messageIndex = 0;
unsigned int messageCount = 0;


//CONFIG
#ifdef DEBUG_MODE
    long timeoutTime = 5000; //5 seconds silence = end song (!!for testing!!)
#else
    long timeoutTime = 10000; //10 seconds silence = end song
#endif

void allocateRecordingMemory() {
    messageBuffer = (uint8_t*)calloc(100000, sizeof(byte));
}

void printByte(byte in) {
    byte i = 8;
    while(i > 0) {
        Serial.print(bitRead(in, i - 1));
        i--;
    }
}

//creates five message bytes from the basic data
Message createMessage(bool onOff, byte pitch, byte velocity, bool isPedal, long songMillis) {
    //time is stored in three bytes - two for the number of seconds, and one for the number of milliseconds (with 4ms precision)
    float songSeconds = (float)songMillis / 1000.0;
    unsigned int wholeSeconds = (unsigned int)songSeconds;
    unsigned int fracSeconds = (unsigned int)(songMillis % 1000);
    fracSeconds /= 4;

    //first byte contains on/off status, and pitch
    byte firstByte = onOff << 7 | (pitch & 0b01111111);
    //second byte contains whether this message is a pedal movement or not, and velocity
    byte secondByte = isPedal << 7 | (velocity & 0b01111111);
    //time bytes, as mentioned above
    byte thirdByte = wholeSeconds >> 8;
    byte fourthByte = wholeSeconds & 0b0000000011111111;
    byte fifthByte = (byte)fracSeconds;

    Message newMessage{
        firstByte,
        secondByte,
        thirdByte,
        fourthByte,
        fifthByte
    };
    #ifdef DEBUG_MODE
    printMessage(newMessage);
    Serial.println("\t" + String(onOff) + "\t" + String(pitch) + "\t" + String(velocity) + "\t    " + String(isPedal) + "\t" + String(songMillis));
    Serial.println("");
    #endif
    return newMessage;
}

//creates a message from the basic data and adds the five resulting bytes to the buffer
//also starts a new song if necessary, and updates the last played note time for calculating timeout
void addMessage(bool onOff, byte pitch, byte velocity, bool isPedal) {
    if(!runningSong) startSong();

    unsigned long songTime = (unsigned long)(millis() - songStartTime);
    
    Message newMessage = createMessage(onOff, pitch, velocity, isPedal, songTime);
    messageBuffer[messageIndex] = newMessage.onOffPitch;
    messageIndex++;
    messageBuffer[messageIndex] = newMessage.isPedalVelocity;
    messageIndex++;
    messageBuffer[messageIndex] = newMessage.timeA;
    messageIndex++;
    messageBuffer[messageIndex] = newMessage.timeB;
    messageIndex++;
    messageBuffer[messageIndex] = newMessage.timeFrac;
    messageIndex++;
    
    messageCount++;
    lastNoteTime = songTime;
}

void printMessage(Message message) {
    Serial.print("  ");
    printByte(message.onOffPitch);
    Serial.print("  ");
    printByte(message.isPedalVelocity);
    Serial.print("  ");
    printByte(message.timeA);
    Serial.print("  ");
    printByte(message.timeB);
    Serial.print("  ");
    printByte(message.timeFrac);
    Serial.println("");
}

void startSong()
{
    #ifdef DEBUG_MODE
    Serial.println("====================STARTING SONG===================");
    #endif
    songStartTime = millis();
    runningSong = true;
}

//resets everything for a new song, and uploads the song if there have been messages entered
void endSong()
{
    if(messageCount > 10) {
        bool success = uploadRecording(messageBuffer, messageCount);
        if(success) {
          wifiSuccessFlash();
        } else {
          errorFlash();
        }
    } else {
      #ifdef DEBUG_MODE
      Serial.println("Message count too low (" + String(messageCount) + ") - skipping upload");
      #endif
    }

    #ifdef DEBUG_MODE
    Serial.println("====================ENDING SONG===================");
    #endif
    messageIndex = 0;
    messageCount = 0;
    runningSong = false;
}