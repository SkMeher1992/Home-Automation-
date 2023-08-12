
#define BLYNK_PRINT Serial


/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL68dSGTIRz"
#define BLYNK_TEMPLATE_NAME "My Home"
#define BLYNK_AUTH_TOKEN "bx87Zi6LjAvS4o7aDuxt4uYWxUES-bZj"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char ssid[] = "M2";
char pass[] = "1777955540";

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;   
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
Servo door;
Servo lock;

#define bazar D0
void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  door.attach(D2); //S1
  lock.attach(D1); //S2
  door.write(150);
  lock.write(90);
  pinMode(bazar, OUTPUT);
  digitalWrite(bazar, LOW);
}

BLYNK_WRITE(V9)
{
  int state1 = param.asInt(); 
  Serial.println(state1); 
  if(state1==1){
      Serial.println("Online Access!");
      delay(100);
      lock.write(0);
      Serial.println("Lock Open");
      delay(1000);
      door.write(0);
      Serial.println("Door Open");
  }
  else{
    door.write(150);
        Serial.println("Lock Close");
        delay(1500);
        Serial.println("Door Close");
        lock.write(90);
  }
}

void loop()
{
  Blynk.run();

  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "16121110627" || tag =="19323414225") {
      Serial.println("Access Granted!");
      delay(100);
      lock.write(0);
      Serial.println("Lock Open");
      delay(1000);
      door.write(0);
      Serial.println("Door Open");

      delay(5000);
    } else {
      Serial.println("Access Denied!");
      digitalWrite(bazar, HIGH); // Send 1KHz sound signal...
      delay(1000);        // ...for 1 sec
      digitalWrite(bazar, LOW);    // Stop sound...
      delay(100); 
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

       door.write(150);
        Serial.println("Lock Close");
        delay(1500);
        Serial.println("Door Close");
        lock.write(90);
  }