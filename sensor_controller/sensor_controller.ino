/* Zach Grasso
 * Started June 9, 2021
 * 
 * Working with a MKR1010 and Bosch BME280 hardware
 * Measuring in Farenheit, Relative Humidity (RA), and wanting to change to atmospheres
 */
#include "passwords.h"
#include <BME280I2C.h>
#include <Wire.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <ArduinoECCX08.h>
#include <ArduinoBearSSL.h>
#include <ArduinoCloudProviderExamples.h>

#define SERIAL_BAUD 9600

BME280I2C bme;

void printSoilMoisture();
int soilMoistureValue = 0;



void setup()
{
  Serial.begin(SERIAL_BAUD);

  while (!Serial) {} // Wait

  Wire.begin();

  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch (bme.chipModel())
  {
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("attempting to connect to: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.println("WiFi Connected");

}


void loop()
{

  printBME280Data(&Serial);
  printSoilMoisture();
  delay(20000);
}

//SoilMoistureValue dry = 1023 and wet = 0
void printSoilMoisture()
{ 
  soilMoistureValue = analogRead(A0);     //soil sensor on Analog0
  Serial.print("Moisture : ");
  Serial.println(soilMoistureValue);
}

void printBME280Data(Stream* client)
{
  float temp(NAN), hum(NAN), pres(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Fahrenheit);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  client->print("Temp: ");
  client->print(temp);
  client->print("°F");
  client->print("\t\tHumidity: ");
  client->print(hum);
  client->print("% RH");
  client->print("\t\tPressure: ");
  client->print(pres);
  client->println("Pa");

}
