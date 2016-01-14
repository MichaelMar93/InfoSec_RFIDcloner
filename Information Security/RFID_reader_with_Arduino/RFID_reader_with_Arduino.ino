
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10 //slave pin
#define RST_PIN 5 //reset pin

MFRC522 MFRC522(SS_PIN, RST_PIN);    // Create MFRC522 instance
MFRC522::MIFARE_Key key;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  MFRC522.PCD_Init();
  Serial.println("scan PIDC to see UID and type ...");

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
}
int block = 2; //block gebruikt om naartoe te schrijven en nadien te lezen.

byte blockcontent[16] = {"makecourse_____"}; //block met 16 bytes te schrijven in één van de 64 card blocks.

byte readbackblock[18]; //gebruikt om een block uit te lezen.

void loop()
{
      /*Contact maken met kaart/tag*/
      
  //look for new cards
  if ( ! MFRC522.PICC_IsNewCardPresent())
  {
    return; //go to start of loop if there is no card present
  }
  //select one of the cards
  if ( ! MFRC522.PICC_ReadCardSerial())
  {
    return; // uf ReadCardSerial returns 1, the "uid" struct contains the ID of the read card.
  }
  //dump debug info about the card, PICC_Hholta() is automatically called.
  Serial.println("card selected");
      
      /*Schrijven naar en lezen van een blok op de kaart */
      
  writeBlock(block, blockcontent);


  readBlock(block, readbackblock);
  Serial.print("read block: ");
  for (int j = 0; j<16;j++)
  {
    Serial.write(readbackblock[j]);
  }
  Serial.println("");
}

int writeBlock(int blockNumber, byte arrayAddress[]) 
{
  
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  if (blockNumber > 2 && (blockNumber+1)%4 == 0){Serial.print(blockNumber);Serial.println(" is a trailer block:");return 2;}
  Serial.print(blockNumber);
  Serial.println(" is a data block:");
  
  /*****************************************authentication of the desired block for access***********************************************************/
  byte status = MFRC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(MFRC522.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed: ");
         Serial.println(MFRC522.GetStatusCodeName(status));
         return 3;
  }
  
  /*****************************************writing the block***********************************************************/
        
  status = MFRC522.MIFARE_Write(blockNumber, arrayAddress, 16);
  if (status != MFRC522::STATUS_OK) {
           Serial.print("MIFARE_Write() failed: ");
           Serial.println(MFRC522.GetStatusCodeName(status));
           return 4;
  }
  Serial.println("block was written");
  }



int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;

  /*****************************************authentication of the desired block for access***********************************************************/
  
  byte status = MFRC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(MFRC522.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed (read): ");
         Serial.println(MFRC522.GetStatusCodeName(status));
         return 3;
  }
  /*****************************************reading a block***********************************************************/
        
  byte buffersize = 18;
  status = MFRC522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);
  if (status != MFRC522::STATUS_OK)  {
          Serial.print("MIFARE_read() failed: ");
          Serial.println(MFRC522.GetStatusCodeName(status));
          return 4;
          }
  Serial.println("block was read");  
  
}


