/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  NOTE: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL68dSGTIRz"
#define BLYNK_TEMPLATE_NAME "My Home"
#define BLYNK_AUTH_TOKEN "bx87Zi6LjAvS4o7aDuxt4uYWxUES-bZj"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "DHT.h"
// Set password to "" for open networks.
char ssid[] = "Sharlok";
char pass[] = "15112000";


#define DHTPIN D5 
#define DHTTYPE DHT11 

int gash;     
#define Light1 D0
#define Light2 D1
#define Fan D2
#define smokeA0 A0


unsigned long currentTime;
unsigned long lastRunTime;
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(Light1, OUTPUT);
  pinMode(Light2, OUTPUT);
  pinMode(Fan, OUTPUT);
  digitalWrite(Light1, LOW);
  digitalWrite(Light2, LOW);
  digitalWrite(Fan, LOW); 
   dht.begin();
}

BLYNK_WRITE(V1)
{
  int state1 = param.asInt(); 
  Serial.println(state1); 
  if(state1==1){
    digitalWrite(Light1, HIGH);
    Serial.println("LED1 High");
  }
  else{
    digitalWrite(Light1, LOW);
    Serial.println("LED1 Low");
  }
}

BLYNK_WRITE(V2)
{
  int state2 = param.asInt(); 
  Serial.println(state2); 
  if(state2==1){
    digitalWrite(Light2, HIGH);
    Serial.println("LED2 High");
  }
  else{
    digitalWrite(Light2, LOW);
    Serial.println("LED2 Low");
  }
}

BLYNK_WRITE(V3)
{
  int state3 = param.asInt(); 
  Serial.println(state3); 
  if(state3==1){
    digitalWrite(Fan, HIGH);
    Serial.println("LED3 High");
  }
  else{
    digitalWrite(Fan, LOW);
    Serial.println("LED3 Low");
  }
}

void loop()
{

  currentTime = millis();
  if (currentTime - lastRunTime >= 5000)
  {
    lastRunTime = currentTime;
    gash= analogRead(smokeA0);
    Serial.println(gash);
    Blynk.virtualWrite(V5, gash);


    float h = dht.readHumidity();
   float t = dht.readTemperature();
   // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
    // runs BlynkTimer
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V6, 0);
    Blynk.virtualWrite(V7, 0);
    return;
  }
  Serial.print("Humidity:");
  Serial.print(h);
  Serial.println("");
  Serial.print("Temperature:(c) ");
  Serial.print(t);
  Serial.println("");
  Serial.print("Temperature:(f) ");
  Serial.print(f);
  Serial.println("");
  Blynk.virtualWrite(V4, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, f);
  }

  Blynk.run();
   

  }