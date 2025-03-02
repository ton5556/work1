#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>
#include "Magellan_BC95_lite.h"  // Magellan IoT library

// Define software serial for PZEM-004T (RX, TX)
SoftwareSerial pzemSerial(8, 9);  // Adjust pins as needed
PZEM004Tv30 pzem(pzemSerial);

// Define software serial for NB-IoT module (RX, TX)
SoftwareSerial nbSerial(0, 1);  // Adjust pins as needed

Magellan_BC95_lite magel;  // Magellan IoT object

void setup() {
  Serial.begin(115200);
  pzemSerial.begin(9600);
  nbSerial.begin(9600);

  Serial.println("Initializing Magellan IoT...");
  magel.begin();  // Initialize Magellan

  Serial.println("Setup complete. Sending data...");
}

void loop() {
  // Read PZEM-004T values
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();

  if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy)) {
    Serial.println("Failed to read from PZEM-004T");
    return;
  }

  // Print values to Serial Monitor
  Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
  Serial.print("Current: "); Serial.print(current); Serial.println(" A");
  Serial.print("Power: "); Serial.print(power); Serial.println(" W");
  Serial.print("Energy: "); Serial.print(energy); Serial.println(" kWh");

  // Format payload in JSON
  String payload = "{";
  payload += "\"Voltage\":" + String(voltage) + ",";
  payload += "\"Current\":" + String(current) + ",";
  payload += "\"Power\":" + String(power) + ",";
  payload += "\"Energy\":" + String(energy);
  payload += "}";

  Serial.println("Sending to Magellan: " + payload);

  // Send data to Magellan IoT platform
  magel.report(payload);

  delay(60000);  // Send data every 60 seconds
}
