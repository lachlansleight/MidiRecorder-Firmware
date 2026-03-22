#include "Connectivity.h"
#include "Flags.h"

//Replace these with your WiFi credentials (2.4GHz only)
//Also...please don't come to my house and hack my WiFi with these!
const char* ssid     = "Realtime";
const char* password = "east4east";
String server = "https://piano.of.glass";
//String server = "http://192.168.1.100:3088";
String path = "/api/upload";
String mac = WiFi.macAddress();
WiFiClientSecure secureClient;

extern const uint8_t x509_crt_bundle_start[] asm("_binary_x509_crt_bundle_start");

bool setupNetwork()
{
    //connect to WiFi
    #ifdef DEBUG_MODE
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.print(" with MAC address ");
    Serial.println(mac);    
    #endif
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        #ifdef DEBUG_MODE
        Serial.print(".");
        #endif
    }
    // ESP32 core 2.x expects only the bundle pointer.
    secureClient.setCACertBundle(x509_crt_bundle_start);

    #ifdef DEBUG_MODE
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    #endif

    return true;
}

//uploads the current song buffer to the server to be further processed
bool uploadRecording(uint8_t* messageBuffer, unsigned int messageCount)
{
    if(WiFi.status() != WL_CONNECTED){
      #ifdef DEBUG_MODE
      Serial.println("WiFi Disconnected");
      #endif
      return false;
    }

    //boolean manualUpload = ButtonUpload.down;
    
    HTTPClient http;

    #ifdef DEBUG_MODE
    Serial.println("Making request to " + (server + path));
    #endif
    http.begin(secureClient, (server + path).c_str());
    
    //data
    http.addHeader("Content-Type", "application/octet-stream");
    http.addHeader("Authorization", "Basic " + mac);
    int httpResponseCode = http.POST(messageBuffer, messageCount * 5);
    
    if (httpResponseCode>0) {
        if(httpResponseCode >= 200 && httpResponseCode <= 299) {
            #ifdef DEBUG_MODE
            String payload = http.getString();
            Serial.print("HTTP Response: ");
            Serial.print(httpResponseCode);
            Serial.print(" - ");
            Serial.println(payload);

            #endif
            http.end();
            return true;
        } else {
            if(httpResponseCode >= 500) {
                #ifdef DEBUG_MODE
                Serial.println("Error - server error " + String(httpResponseCode));
                #endif
                http.end();
                return false;
            } else {
                #ifdef DEBUG_MODE
                Serial.println("Error - request error " + String(httpResponseCode));
                #endif
                http.end();
                return false;
            }
        }
    }
    else {
        #ifdef DEBUG_MODE
        if(httpResponseCode == -1) Serial.println("Error - Connection_Refused (Code -1)");
        else if(httpResponseCode == -2) Serial.println("Error - Send Header Failed (Code -2)");
        else if(httpResponseCode == -3) Serial.println("Error - Send Payload Failed (Code -3)");
        else if(httpResponseCode == -4) Serial.println("Error - Not Connected (Code -4)");
        else if(httpResponseCode == -5) Serial.println("Error - Connection Lost (Code -5)");
        else if(httpResponseCode == -6) Serial.println("Error - No Stream (Code -6)");
        else if(httpResponseCode == -7) Serial.println("Error - No Http Server (Code -7)");
        else if(httpResponseCode == -8) Serial.println("Error - Not Enough RAM (Code -8)");
        else if(httpResponseCode == -9) Serial.println("Error - Encoding (Code -9)");
        else if(httpResponseCode == -10) Serial.println("Error - Stream Write (Code -10)");
        else if(httpResponseCode == -11) Serial.println("Error - Read Timeout (Code -11)");
        else Serial.println("Error - Unexpected error code " + String(httpResponseCode));
        #endif

        //flash LED white/red, then fade
        http.end();
        return false;
    }
    // Free resources
    http.end();
    return true;
}