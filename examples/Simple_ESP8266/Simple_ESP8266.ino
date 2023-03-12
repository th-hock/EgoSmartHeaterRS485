/****************************************************************************************************************************
  Simple_ESP8266.ino - Simple example for ESP8266 to control an EGO Smart Heater device

  Built by Thomas Hock https://github.com/th-hock
  Licensed under MIT license
 *****************************************************************************************************************************/

// EGO Smart Heater control
#define DERE_PIN D1     // DE and RE Pin
#define ENERGY_RX_PIN D2  // RO Pin
#define ENERGY_TX_PIN D3  // DI Pin

#include <SoftwareSerial.h>
// use SW-serial, since ESP does not provide additional HW serial interfaces
SoftwareSerial swSerial;
#include <EgoSmartHeaterRS485.h>
//Initialize the EGO SmartHeater instance with the DE-RE pin number
EgoSmartHeaterRS485 Heater(DERE_PIN);

void setup() {
  Serial.begin(115200);

  // communicate with Modbus slave via SW serial
  swSerial.begin(EGO_SH_RS485_SERIAL_BAUD, SWSERIAL_8E1, ENERGY_RX_PIN, ENERGY_TX_PIN);
  // attach the Smart Heater to the SW-serial instance and start communication
  Heater.begin(swSerial);
  delay(5000);
  Serial.println("\nHeater serial started");

  // retrieve some device details, refer to the protocol description for all the details
  Serial.print("VendorName: ");
  Serial.println(Heater.getVendorName());
  delay(10);
  Serial.print("ProductName: ");
  Serial.println(Heater.getProductName());
  delay(10);
  Serial.print("SerialNumber: ");
  Serial.println(Heater.getSerialNumber());
  delay(10);
  Serial.print("RelaisCount: ");
  Serial.println(Heater.getRelaisCount());
  delay(10);
}

void loop() {
  // switch the heater to 500W consumption
  Heater.setPowerNominalValue(500);
  // wait 30 seconds and renew the power nominal value setting
  // if it is not renewed within 60 seconds the heater is automatically turned off  
  Serial.println("Heater switched to 500W");
  Serial.print("Relais Status: ");
  Serial.println(Heater.getRelaisStatus());
  delay(30000);
}
