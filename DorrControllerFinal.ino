/* Read RFID Tag with RC522 RFID Reader
    Made by miliohm.com
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;


Servo door;
Servo lock;
#define bazar D0
void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  door.attach(D2); //S1
  lock.attach(D1); //S2
  door.write(150);
  lock.write(90);
  pinMode(bazar, OUTPUT);
  digitalWrite(bazar, LOW);

}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "16121110627"||tag == "19323414225") {
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