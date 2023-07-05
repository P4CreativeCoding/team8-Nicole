/*--------------------------
  Code Anätze von Rudy Schlaf for www.makecourse.com
	DATE:		2/2014
----------------------------*/

#include <SPI.h>//include the SPI bus library
#include <MFRC522.h>//include the RFID reader library

#define SS_PIN 10  //slave select pin
#define RST_PIN 5  //reset pin
MFRC522 mfrc522(SS_PIN, RST_PIN); // instatiate a MFRC522 reader object
MFRC522::MIFARE_Key key; //create a MIFARE_Key struct named 'key', which will hold the card information

const int ledPin = 7; // LED uses Pin 7

void setup() {
        Serial.begin(9600); // Initialize serial communications with the PC
        SPI.begin(); // Init SPI bus
        mfrc522.PCD_Init(); // Init MFRC522 card (PCD means: proximity coupling device)
 
        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF; //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
          pinMode(ledPin, OUTPUT);
        }
}

int block=2; //this is the block number. The code  will write into it and then read it
                          
byte readbackblock[18]; //This array is used for reading out a block. The MIFARE_Read method requires a buffer that is at least 18 bytes to hold the 16 bytes of a block

void loop()
{

  /*---------------------establishing contact with the tag----------------------------*/
        
  // This will look for new cards (PICC means: proximity integrated circuit card)
	if ( ! mfrc522.PICC_IsNewCardPresent()) { //if PICC_IsNewCardPresent returns 1, a new card has been found and we continue
		return; //if it did not find a new card it returns a 0 and the code returna to the start of the loop
	}

	// This will select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) { //if PICC_ReadCardSerial returns 1, the "uid" struct contains the ID of the read card
		return; //if it returns a 0 something went wrong and it returns to the start of the loop
	}

  /*---------------------writing and reading a block on the card------------------------*/
         
         readBlock(block, readbackblock); //read the block back
         for (int j=0 ; j<16 ; j++) //print the block contents
         {
           Serial.write (readbackblock[j]); //Serial.write() transmits the ASCII numbers as human readable characters to serial monitor
         }
         Serial.println("");
         digitalWrite(ledPin, HIGH);
}

  /*--------------------------writing the block-----------------------------------------*/

int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3; //determine trailer block for the sector

  /*------------------------authentication of the desired block for access--------------*/  

  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) { // if the authentication is not approved, give me the following line
         Serial.print("PCD_Authenticate() failed (read): ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }

  /*-----------------------reading a block--------------------------------------------*/
        
  byte buffersize = 18; //it needs to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize); //&buffersize is a pointer to the buffersize variable, MIFARE_Read requires a pointer instead of just a number
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(status));
          return 4; //return "4" as error message
  }
}