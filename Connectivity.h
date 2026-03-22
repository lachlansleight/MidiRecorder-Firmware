#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include <esp_crt_bundle.h>

bool setupNetwork();
bool uploadRecording(uint8_t* messageBuffer, unsigned int messageCount);

#endif