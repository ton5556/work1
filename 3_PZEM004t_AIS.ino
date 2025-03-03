#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>
#include "Magellan_BC95_lite.h"  // Magellan IoT library

// Define software serial for three PZEM-004T modules
SoftwareSerial pzemSerial1(10, 11);  // PZEM-1
SoftwareSerial pzemSerial2(7, 6);  // PZEM-2
SoftwareSerial pzemSerial3(4, 5);  // PZEM-3

PZEM004Tv30 pzem1(pzemSerial1);
PZEM004Tv30 pzem2(pzemSerial2);
PZEM004Tv30 pzem3(pzemSerial3);

SoftwareSerial nbSerial(0,1);
Magellan_BC95_lite magel;  

// Struct to hold sensor data
struct SensorData {
  float voltage;
  float current;
  float power;
  float energy;
  float frequency;
};

// Array to store data from all three sensors
SensorData sensorData[3];

void setup() {
  Serial.begin(115200);
  pzemSerial1.begin(9600);
  pzemSerial2.begin(9600);
  pzemSerial3.begin(9600);
  nbSerial.begin(9600);

  Serial.println("Initializing Magellan IoT...");
  magel.begin();  // Initialize Magellan

  Serial.println("Setup complete. Sending data...");
}

void loop() {
  // Read values from PZEM-004T modules and store in array
  sensorData[0] = { pzem1.voltage(), pzem1.current(), pzem1.power(), pzem1.energy(), pzem1.frequency() };
  sensorData[1] = { pzem2.voltage(), pzem2.current(), pzem2.power(), pzem2.energy(), pzem2.frequency() };
  sensorData[2] = { pzem3.voltage(), pzem3.current(), pzem3.power(), pzem3.energy(), pzem3.frequency() };

  // Check if any sensor failed to read
  for (int i = 0; i < 3; i++) {
    if (isnan(sensorData[i].voltage) || isnan(sensorData[i].current) ||
        isnan(sensorData[i].power) || isnan(sensorData[i].energy) || isnan(sensorData[i].frequency)) {
      Serial.println("Failed to read from one or more PZEM-004T modules");
      return;
    }
  }

  // Format payload in JSON using String
  String payload = "{";
  payload += "\"PZEM1\":{";
  payload += "\"Voltage\":" + String(sensorData[0].voltage) + ",";
  payload += "\"Current\":" + String(sensorData[0].current) + ",";
  payload += "\"Power\":" + String(sensorData[0].power) + ",";
  payload += "\"Energy\":" + String(sensorData[0].energy) + ",";
  payload += "\"Frequency\":" + String(sensorData[0].frequency);
  payload += "},";

  payload += "\"PZEM2\":{";
  payload += "\"Voltage\":" + String(sensorData[1].voltage) + ",";
  payload += "\"Current\":" + String(sensorData[1].current) + ",";
  payload += "\"Power\":" + String(sensorData[1].power) + ",";
  payload += "\"Energy\":" + String(sensorData[1].energy) + ",";
  payload += "\"Frequency\":" + String(sensorData[1].frequency);
  payload += "},";

  payload += "\"PZEM3\":{";
  payload += "\"Voltage\":" + String(sensorData[2].voltage) + ",";
  payload += "\"Current\":" + String(sensorData[2].current) + ",";
  payload += "\"Power\":" + String(sensorData[2].power) + ",";
  payload += "\"Energy\":" + String(sensorData[2].energy) + ",";
  payload += "\"Frequency\":" + String(sensorData[2].frequency);
  payload += "}";

  payload += "}";  // Close main JSON object

  Serial.println("Sending to Magellan: ");
  Serial.println(payload);

  // Send data to Magellan IoT platform
  if (magel.report(payload)) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Failed to send data!");
  }

  delay(6000);  // Send data every 60 seconds
}
