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
MFRC522::MIFARE_Key facKey;
MFRC522::MIFARE_Key keyA;
MFRC522::MIFARE_Key keyB;

//write blocks for 1K card
const int write_block[] = {0,1,2,4,5,6,8,9,10,12,13,14,16,17,18,20,21,22,24,25,26,28,29,30,32,33,34,36,37,38,40,41,42,44,45,46,48,49,50,52,53,54,56,57,58,60,61,62};

void setup() {
  //generate factory key voor zowel A als B
  for (byte i = 0; i < 6; i++) {
        facKey.keyByte[i] = 0xFF;
  }
  //nieuwe key a en b voor demo
  //key a bytes = 110 111 112 113 114 115  
  keyA.keyByte[0] = 0x6e;
  keyA.keyByte[1] = 0x6f;
  keyA.keyByte[2] = 0x70;
  keyA.keyByte[3] = 0x71;
  keyA.keyByte[4] = 0x72;
  keyA.keyByte[5] = 0x73;

  //key b bytes = 120 121 122 123 124 125  
  keyB.keyByte[0] = 0x78;
  keyB.keyByte[1] = 0x79;
  keyB.keyByte[2] = 0x7a;
  keyB.keyByte[3] = 0x7b;
  keyB.keyByte[4] = 0x7c;
  keyB.keyByte[5] = 0x7d;


  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Scan PICC to see UID and type...");
}

void loop() {
  
}

/*Dump card to serial*/
void dumpToSerial(){
  if(! mfrc522.PICC_IsNewCardPresent()){
    return; 
  }

  if(! mfrc522.PICC_ReadCardSerial()){
    return;
  }

  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

byte blockcontent1[16] = {"AP_Hogeschool__"};
byte blockcontent2[16] = {"2015-2016______"};

void testWrite(int keyBlock, int writeBlock, byte* content, MFRC522::MIFARE_Key _key){
  if(! mfrc522.PICC_IsNewCardPresent()){
    return; 
  }
  if(! mfrc522.PICC_ReadCardSerial()){
    return;
  }
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, keyBlock, _key, &(mfrc522.uid));
  
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed: ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return;//return "3" as error message
  }
  status = mfrc522.MIFARE_Write(writeBlock, content, 16);//valueBlockA is the block number, MIFARE_Write(block number (0-15), byte array containing 16 values, number of bytes in block (=16))
  if (status != MFRC522::STATUS_OK) {
           Serial.print("MIFARE_Write() failed: ");
           Serial.println(mfrc522.GetStatusCodeName(status));
           return;//return "4" as error message
  } 
}
void readWrite(){
  
}
