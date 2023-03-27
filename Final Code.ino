#include <Bridge.h>
#include <HttpClient.h>

// Replace these with your sensor pins
const int soilMoisturePin = A0;
const int lightSensorPin = A1;
const int temperatureSensorPin = A2;

// Replace with your PushingBox Device ID
const String deviceId = "your_device_id_here";

void setup() {
  Bridge.begin();
  Serial.begin(9600);

  pinMode(soilMoisturePin, INPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(temperatureSensorPin, INPUT);
}

void loop() {
  int soilMoisture = analogRead(soilMoisturePin);
  int light = analogRead(lightSensorPin);
  float temperature = readTemperature(temperatureSensorPin);

  sendDataToPushingBox(deviceId, soilMoisture, light, temperature);

  delay(60000); // Wait for 1 minute (60000 ms) before sending data again
}

float readTemperature(int pin) {
  int analogValue = analogRead(pin);
  float voltage = analogValue * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100; // Convert to Celsius
  return temperature;
}

void sendDataToPushingBox(String deviceId, int soilMoisture, int light, float temperature) {
  HttpClient client;
  String url = "http://api.pushingbox.com/pushingbox?devid=" + deviceId + "&soilMoisture=" + String(soilMoisture) + "&light=" + String(light) + "&temperature=" + String(temperature);

  client.get(url);

  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  Serial.println();
  Serial.println("Data sent");
}
