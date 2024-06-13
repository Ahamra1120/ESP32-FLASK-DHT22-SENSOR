#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 27     // Pin di mana sensor DHT22 terhubung
#define DHTTYPE DHT22   // DHT 22 

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "AHR"; // Alamat SSID menyesuaikan
const char* password = "12345678"; // Alamat Password menyesuaikan
const char* serverName = "http://192.168.15.162:5000/sensor/data"; // Alamat IP menyesuaikan

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(5000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "temperature=" + String(t, 2) + "&humidity=" + String(h, 2);

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      Serial.println(httpResponseCode);
      Serial.println(postData);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }

  delay(1000); // Kirim data setiap 1 detik
}
