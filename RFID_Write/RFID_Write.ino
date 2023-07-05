/*--------------------------
  Code Anätze von Rudy Schlaf for www.makecourse.com
	DATE:		2/2014
----------------------------*/

#include <CapacitiveSensor.h>

#define PIN_LED           8
#define PIN_TOUCH_SEND    6
#define PIN_TOUCH_RECEIVE 7
#define PIN_TO_ACTIVATE 3 

CapacitiveSensor touchSensor = CapacitiveSensor(PIN_TOUCH_SEND, PIN_TOUCH_RECEIVE);

#include <SPI.h> // include the SPI bus library
#include <MFRC522.h> // include the RFID reader library

#define SS_PIN 10  // slave select pin
#define RST_PIN 5  // reset pin
MFRC522 mfrc522(SS_PIN, RST_PIN); // instatiate a MFRC522 reader object.
MFRC522::MIFARE_Key key; //create a MIFARE_Key struct named 'key', which will hold the card information

const int ledPin = 7;

void setup() {
        Serial.begin(9600); // Initialize serial communications with the PC
        SPI.begin(); // Init SPI bus
        mfrc522.PCD_Init(); // Init MFRC522 card (PCD means: proximity coupling device)
        
        // Prepare the security key for the read and write functions - all six key bytes are set to 0xFF at chip delivery from the factory.
        // Since the cards in the kit are new and the keys were never defined, they are 0xFF
        // if we had a card that was programmed by someone else, we would need to know the key to be able to access it. This key would then need to be stored in 'key' instead.

        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF; //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
        }

        
    pinMode(PIN_LED, OUTPUT);
    touchSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);

    pinMode(PIN_TO_ACTIVATE, OUTPUT);

}

int block=2; //this is the block number the code will write into and then read.
                          
byte blockcontent[16] = {"06-07-2023 Start"}; //set the start date of period/ track it when the user use the sensitive touch ---> HARD CODE
byte readbackblock[18]; //This array is used for reading out a block.

void loop()
{
  /*------------LED and vibration when sensititve touch is used----------------*/

    long start = millis();
    long total =  touchSensor.capacitiveSensor(30);

    if (total < 1000) {
        digitalWrite(PIN_LED, HIGH); //LED deactivate if touch sensor is used
        digitalWrite(PIN_TO_ACTIVATE, HIGH);  // Pin activate

    } else {
        digitalWrite(PIN_LED, LOW); //LED deactivate if touch sensor is not used
        digitalWrite(PIN_TO_ACTIVATE, LOW);  // Pin deactivate
    }

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
         
  writeBlock(block, blockcontent); //the blockcontent array is written into the card block

  readBlock(block, readbackblock); //read the block back
         for (int j=0 ; j<16 ; j++) //print the block contents
         {
           Serial.write (readbackblock[j]); //Serial.write() transmits the ASCII numbers as human readable characters to serial monitor
         }
         Serial.println("");
         digitalWrite(ledPin, HIGH);
}

int writeBlock(int blockNumber, byte arrayAddress[]) 
{
  //this makes sure that we only write into data blocks. Every 4th block is a trailer block for the access/security info.
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3; //determine trailer block for the sector
  if (blockNumber > 2 && (blockNumber+1)%4 == 0){Serial.print(blockNumber);Serial.println(" is a trailer block:");return 2;}v//block number is a trailer block (modulo 4); quit and send error code 2
  
  /*----------------------authentication of the desired block for access-------------------------*/
  
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  
  if (status != MFRC522::STATUS_OK) { // if the authentication is not approved, give me the following line
         Serial.print("PCD_Authenticate() failed: ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }

  /*------------------------writing the block-------------------------------*/
        
  status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16); //valueBlockA is the block number, MIFARE_Write(block number (0-15)

  if (status != MFRC522::STATUS_OK) { // if writing the block failed, give me the following line
           Serial.print("MIFARE_Write() failed: ");
           Serial.println(mfrc522.GetStatusCodeName(status));
           return 4;//return "4" as error message
  }
}


int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3; //determine trailer block for the sector


  /*------------------------authentication of the desired block for access---------------------*/

 byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) { // if the authentication is not approved, give me the following line
         Serial.print("PCD_Authenticate() failed (read): ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }


  /*---------------------------reading a block---------------------------*/
        
  byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(status));
          return 4;//return "4" as error message
  }
}
