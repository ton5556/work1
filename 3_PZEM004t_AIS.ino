#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>
#include "Magellan_BC95_lite.h"  // Magellan IoT library

// Define software serial for three PZEM-004T modules
SoftwareSerial pzemSerial1(8, 9);  // PZEM-1
SoftwareSerial pzemSerial2(7, 6);  // PZEM-2
SoftwareSerial pzemSerial3(4, 5);  // PZEM-3

PZEM004Tv30 pzem1(pzemSerial1);
PZEM004Tv30 pzem2(pzemSerial2);
PZEM004Tv30 pzem3(pzemSerial3);

// Define software serial for NB-IoT module
SoftwareSerial nbSerial(0, 1);  // Adjust pins as needed
Magellan_BC95_lite magel;  // Magellan IoT object

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
  // Read values from PZEM-004T modules
  float voltage1 = pzem1.voltage();
  float current1 = pzem1.current();
  float power1 = pzem1.power();
  float energy1 = pzem1.energy();
  float frequency1 = pzem1.frequency();

  float voltage2 = pzem2.voltage();
  float current2 = pzem2.current();
  float power2 = pzem2.power();
  float energy2 = pzem2.energy();
  float frequency2 = pzem2.frequency();

  float voltage3 = pzem3.voltage();
  float current3 = pzem3.current();
  float power3 = pzem3.power();
  float energy3 = pzem3.energy();
  float frequency3 = pzem3.frequency();

  if (isnan(voltage1) || isnan(current1) || isnan(power1) || isnan(energy1) || isnan(frequency1) ||
      isnan(voltage2) || isnan(current2) || isnan(power2) || isnan(energy2) || isnan(frequency2) ||
      isnan(voltage3) || isnan(current3) || isnan(power3) || isnan(energy3) || isnan(frequency3)) {
    Serial.println("Failed to read from one or more PZEM-004T modules");
    return;
  }

  // Format JSON payload efficiently
  char payload[300];  // Allocate buffer
  snprintf(payload, sizeof(payload),
           "{"
           "\"PZEM1\":{\"Voltage\":%.2f,\"Current\":%.2f,\"Power\":%.2f,\"Energy\":%.2f,\"Frequency\":%.2f},"
           "\"PZEM2\":{\"Voltage\":%.2f,\"Current\":%.2f,\"Power\":%.2f,\"Energy\":%.2f,\"Frequency\":%.2f},"
           "\"PZEM3\":{\"Voltage\":%.2f,\"Current\":%.2f,\"Power\":%.2f,\"Energy\":%.2f,\"Frequency\":%.2f}"
           "}",
           voltage1, current1, power1, energy1, frequency1,
           voltage2, current2, power2, energy2, frequency2,
           voltage3, current3, power3, energy3, frequency3);

  Serial.println("Sending to Magellan: ");
  Serial.println(payload);

  // Send data to Magellan IoT platform
  if (magel.report(payload)) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Failed to send data!");
  }

  delay(60000);  // Send data every 60 seconds
}
