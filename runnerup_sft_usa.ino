/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6YYCpzRE4"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "7KIOmncnJjhtkoHPGOVKYrsJruHkYBEH"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "Protocentral_MAX30205.h"

MAX30205 tempSensor;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "BECONEC";
char pass[] = "Dmarcom3!@";

BlynkTimer timer;

const int buzzerPin = 12; // Define the buzzer pin

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // Read temperature
  float temp = tempSensor.getTemperature();

  // Send temperature data to Blynk
  Blynk.virtualWrite(V2, temp);

  // Print temperature to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  // You can send any value at any time.
  // Please don't send more than 10 values per second.
  Blynk.virtualWrite(V4, millis() / 1000);

  // Activate the buzzer if temperature is greater than 25°C
  if (temp > 38.0) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Wire.begin();

  // Initialize temperature sensor
  while(!tempSensor.scanAvailableSensors()){
    Serial.println("Couldn't find the temperature sensor, please connect the sensor." );
    delay(30000);
  }
  tempSensor.begin();   // set continuous mode, active mode

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as output
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}