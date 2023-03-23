#include <Bridge.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <HttpClient.h>

#define MOISTURE_SENSOR A0
#define TEMP_SENSOR_PIN 2
#define LIGHT_SENSOR_ADDR 0x39

#define PUSHINGBOX_DEVICE_ID "YOUR_DEVICE_ID"

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(LIGHT_SENSOR_ADDR, 12345);

void setup() {
  Bridge.begin();
  Serial.begin(9600);

  if (!tsl.begin()) {
    Serial.println("Failed to initialize the TSL2561 light sensor!");
    while (1);
  }

  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
}

void loop() {
  int soilMoisture = readSoilMoisture();
  float temperature = readTemperature();
  float light = readLight();

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println(" %");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Light: ");
  Serial.print(light);
  Serial.println(" lux");

    sendDataToPushingBox(soilMoisture, temperature, light);

  delay(60000); // Read and send data every minute
}

int readSoilMoisture() {
  int sensorValue = analogRead(MOISTURE_SENSOR);
  int soilMoisture = map(sensorValue, 0, 1023, 0, 100);
  return soilMoisture;
}

float readTemperature() {
  // Add code to read temperature from the DS18B20 sensor
}

float readLight() {
  sensors_event_t event;
  tsl.getEvent(&event);

  if (event.light) {
    return event.light;
  } else {
    return -1;
  }
}

void sendDataToPushingBox(int soilMoisture, float temperature, float light) {
  HttpClient client;
  client.noCheckSSL();
  String url = "https://api.pushingbox.com/pushingbox?devid=" + String(PUSHINGBOX_DEVICE_ID);
  url += "&soilMoisture=" + String(soilMoisture);
  url += "&temperature=" + String(temperature);
  url += "&light=" + String(light);

  client.get(url);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
} 
