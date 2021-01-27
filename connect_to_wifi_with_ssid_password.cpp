#include <Arduino.h>
#include <WiFi.h>

const char *WIFI_SSID = "your-ssid";
const char *WIFI_PASSWORD = "ssid's-password";

void setup() {
    Serial.begin(115200);
    // Station mode
    WiFi.mode(WIFI_STA);
    // Initialize
    WiFi.disconnect();
    // Connect to the specified ssid with password
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // Check connection status
    while (WiFi.status() != WL_CONNECTED) {
        delay(5000);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.print("Try to connect to ");
        Serial.println(WIFI_SSID);
    }    
    Serial.println("WiFi connected !");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    delay(10000);
}
