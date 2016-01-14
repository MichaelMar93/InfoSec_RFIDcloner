/*
Title: RFID cloner
Authors: Michael Marivoet & Ward De Beuckelear
Last revision: 06/01/2015

Description: RFID tag cloner, project for information security 2015-2016 @ Artesis Plantijn Hogeschool Antwerpen


Pins:

SS - 53
RESET - 5 
MOSI - 51 
MISO - 50
SCK - 52

*/
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Scan PICC to see UID and type...");
}

void loop() {
  dumpToSerial();
}
void dumpToSerial(){
  if(! mfrc522.PICC_IsNewCardPresent()){
    return; 
  }

  if(! mfrc522.PICC_ReadCardSerial()){
    return;
  }

  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
