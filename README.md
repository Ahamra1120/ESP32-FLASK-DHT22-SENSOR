# ESP32-FLASK-DHT22-SENSOR
Tugas Technical Assignments SIC 2024<br>
Saya menggunakan Sensor DHT22 (Temprature & Humidity) sebagai Input-nya, ESP32 melakukan POST Data Sensor, dan Flask menampilkan data yang diterima dari ESP32.

# Identitas Diri
Nama : Ahmad Hamra<br>
Tim  : EnviroMind<br>
Asal : MAN 2 Jakarta<br>

# Alat dan bahan
- ESP32
- Sensor DHT22 (Tempratur dan Kelembaban)
- Kabel Jumper

# Pin ESP32 TO SENSOR
| ESP32 | Sensor DHT22       |
|----------------|------------------|
| 5V / 3V| VCC |
| GND | GND |
| 27 | OUT |
# Foto Rangkaian Fisik
![Gambar Proyek](img/Rangkaian_Fisik.jpg)

# Program Arduino IDE
```c++
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
```
Klik tautan berikut untuk melihat kode <br>
[kirim_data.ino](kirim_data/kirim_data.ino)

# Program Python Flask
Program Flask
```python

from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
from datetime import datetime

app = Flask(__name__)
CORS(app)

data_list = []

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/sensor/data', methods=['POST'])
def receive_data():
    temperature = request.form.get('temperature')
    humidity = request.form.get('humidity')

    if temperature is not None and humidity is not None:
        try:
            temperature = float(temperature)
            humidity = float(humidity)
            timestamp = datetime.now().strftime('%d/%m/%Y %H:%M:%S')
            data = {"temperature": temperature, "humidity": humidity, "timestamp": timestamp}
            data_list.append(data)
            return jsonify({"message": "Data received"}), 200
        except ValueError:
            return jsonify({"message": "Invalid data format"}), 400
    else:
        return jsonify({"message": "Missing parameters"}), 400

@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(data_list), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
```
Atau bisa lihat pada link berikut : <br>
[main.py](main.py)

# Tampilan Dashboard Data Sensor DHT22
![Gambar Dashboard](img/Dashboard_Data_Sensor.jpg)
