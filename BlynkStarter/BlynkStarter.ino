/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "sUiVLDEdz785MPaqng-pNojrN3_wLcw9";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials.
char ssid[32] = "I like Elmo cuz he's red";
char pass[32] = "idonotlikewholewheatbread";

#define LED 12

BlynkTimer timer;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;
int duty_cycle = 0;
int state = 0;

int time_count = 0; // timer counter global variable
String content = "";  // null string constant ( an empty string )

void myTimerEvent() // Every 10 ms
{
  if(state){
    ledcWrite(ledChannel, duty_cycle);
  } else{
    ledcWrite(ledChannel, 0);
  }
  
  if (time_count % 25 == 0) { // every 250 ms
    // Do thing that needs to happen every 0.25 seconds
  }
  if (time_count == 100) {
    // Do thing that needs to happen every 1 second
    time_count = 0; // reset time counter
  }
  else {
    // Send serial data to Blynk terminal
    char character;
    while (Serial.available()) { // Check if serial is available every 10 ms
      character = Serial.read();
      content.concat(character);
    }
    if (content != "") {
      Blynk.virtualWrite(3, content);
      content = ""; // Clear String
    }
  }
  time_count += 1; // Increment on every tick
}
void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(10L, myTimerEvent); // 10 ms interval

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);

  //pinMode(LED, OUTPUT);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}

BLYNK_WRITE(V1)
{
  // param is a member variable of the Blynk ADT. It is exposed so you can read it.
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  state = pinValue;
  // Because V1 is a button, pinValue will be a 0 or a 1.
  if (pinValue == 0) {
    // turn LED off
    //Serial.println("off");
  }
  else {
    // turn LED on
    //Serial.println("on");
  }
}

BLYNK_WRITE(V2)
{
  int value = param.asInt();
  duty_cycle = value;
}

// This function is executed whenever Blynk pin V5 requests data from the ESP32
BLYNK_READ(V5) // Widget in the app READS Virtal Pin V5 with the certain frequency
{
  // This command writes Arduino's uptime in seconds to Virtual Pin V5
  Blynk.virtualWrite(5, millis() / 1000);
}
