#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_SERVER "broker.mqtt-dashboard.com" // MQTT Server
#define MQTT_PORT 1883          // MQTT Server Port
#define CLIENT_ID ""            // MQTT client ID
#define MQTT_USERNAME ""        // MQTT username, leave it blank if it is no value
#define MQTT_PASSWORD ""        // MQTT password, leave it blank if it is no value
#define TOPIC ""                // MQTT Topic

#define WIFI_SSID "your-ssid"               // WiFi SSID
#define WIFI_PASSWORD "ssid's-password"     // WiFi preshared key

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

String msgStr = ""; 
const long interval = 20000;    // 上傳資料的間隔時間，20秒。
unsigned long prevMillis = 0;   // 暫存經過時間（毫秒）

void connectMQTT();
void connectWiFi();

void setup() {
    Serial.begin(115200);
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}
void loop() {
    delay(10000);

    if (WiFi.status() != WL_CONNECTED)
        connectWiFi();
    if (!mqttClient.connected()) 
        connectMQTT();

    // 等待20秒
    if (millis() - prevMillis > interval) {
        prevMillis = millis();

        // 組合MQTT訊息
        msgStr = msgStr + (random(0, 300) / 10.0);

        // 宣告字元陣列
        byte arrSize = msgStr.length() + 1;
        char msg[arrSize];

        Serial.print("Publish message: ");
        Serial.println(msgStr);
        msgStr.toCharArray(msg, arrSize);     // 把String字串轉換成字元陣列格式
        mqttClient.publish(TOPIC, msg);       // 發布MQTT主題與訊息
        msgStr = "";
    }
}

void connectWiFi() {
    // Station mode
    WiFi.mode(WIFI_STA);
    // Initialize
    WiFi.disconnect();
    // Connect to the specified ssid with password
    // Check connection status
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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

void connectMQTT() {
    while (!mqttClient.connected()) {
        if (mqttClient.connect(CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("MQTT connected");
        } else {
            Serial.print("failed, rc=");        // return code
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);  // 等5秒之後再重試
        }
    }
}
