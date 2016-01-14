
#include <MFRC522.h>

#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 5
MFRC522 MFRC522(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(9600);
  SPI.begin();
  MFRC522.PCD_Init();
  Serial.println("Scan PICC to see UID and type ...");

}

void loop() {
  if ( ! MFRC522.PICC_IsNewCardPresent())
  {
    return;
  }

  if (! MFRC522.PICC_ReadCardSerial())
  {
    return;
  }

  MFRC522.PICC_DumpToSerial(&(MFRC522.uid));

}
