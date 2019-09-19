/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example runs directly on ESP8266 chip.
 *
 * Note: This requires ESP8266 support package:
 *   https://github.com/esp8266/Arduino
 *
 * Please be sure to select the right ESP8266 module
 * in the Tools -> Board menu!
 *
 * NOTE: SmartConfig might not work in your environment.
 *       Please try basic ESP8266 SmartConfig examples
 *       before using this sketch!
 *
 * Change Blynk auth token to run :)
 * 
 * Plus WiFi-config function into Blynk Mighty D1
 *
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h> // Arduino IDE <1.6.6
#include <PZEM004T.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4e08a6a3122343f5beb5c1c94369b4a7"; //imtbtest01;
//char auth[] = "f82b3aa574924044bcf36597bbab2779"; //imtbtest02; 

char ssid[] = "5630";  //at home;
char pass[] = "27682477";   //at home;

//char ssid[] = "Classroom";  //at I40 Center;
//char pass[] = "ntustclassroom";   //at I40 Center;

//char ssid[] = "Jason iphone";
//char pass[] = "iphone0316";

PZEM004T pzem(&Serial);
IPAddress ip(192,168,1,1);

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pzem.setAddress(ip);
  WiFi.mode(WIFI_STA);

  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (cnt++ >= 10) {
      WiFi.beginSmartConfig();
      while (1) {
        delay(1000);
        if (WiFi.smartConfigDone()) {
          Serial.println();
          Serial.println("SmartConfig: Success");
          break;
        }
        Serial.print("|");
      }
    }
  }

  WiFi.printDiag(Serial);

  Blynk.config(auth);
}

void loop()
{
  Blynk.run();
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);                       // wait for a second
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);                       // wait for a second  
}


BLYNK_READ(V0) {
  float v = pzem.voltage(ip);
  if (v < 0.0) v = 0.0;
  Blynk.virtualWrite(V0,v);
  }

BLYNK_READ(V1) {
  float i = pzem.current(ip);
  if (i < 0.0) i = 0.0;
  Blynk.virtualWrite(V1,i);
  }

BLYNK_READ(V2) {
  float p = pzem.power(ip);
  if (p < 0.0) p = 0.0;
  Blynk.virtualWrite(V2,p);
  }

BLYNK_READ(V3) {
  float e = pzem.energy(ip);
  if (e < 0.0) e = 0.0;
  Blynk.virtualWrite(V3,e/1000);
  }
