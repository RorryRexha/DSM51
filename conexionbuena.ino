#include <ESP8266WiFi.h>
#include "HX711.h"

const char* ssid = "rorry";
const char* password = "huevos11";

const char* serverName = "http://localhost/bascula/index.php";

HX711 scale;

void setup() {
  Serial.begin(115200);
  scale.begin(D1, D5);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  float weight = scale.get_units();
  Serial.println(weight);
  if (WiFi.status() == WL_CONNECTED) {
    sendWeight(weight);
  }
  delay(5000);
}

void sendWeight(float weight) {
  WiFiClient client;
  if (client.connect(serverName, 80)) {
    String postData = "weight=" + String(weight);
    client.println("POST " + String(serverName) + " .HTTP/1.1");
    client.println("Host: " + String(serverName));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(postData.length()));
    client.println();
    client.println(postData);
  }
}
