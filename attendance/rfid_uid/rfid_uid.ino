#include <SPI.h>
#include <MFRC522.h>
#include <Keyboard.h>
  
#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class MFRC522
 
MFRC522::MIFARE_Key key; 

// declares  two arrays (one for the known tags and one for the associated password (indexes  must match))
// declares an int corresponding to the length of the two previous  arrays
// declaring a string to store the UID of the last tag read by the sensor
//  finally, declares a string that maps the DEC to HEX conversion

// String tags[]  = {"53 12 F4 11", "03 9F 2E 11", "tag3"}; // tags consist of four pairs of  hex digits with spaces between them (but NOT at the end)
// String pwds[] = {"AWhiteTagPassWord",  "ABlueTagPassWord", "pwd3"};
// int listSize = 3; //must at all times equal  the number of elements in the previous two arrays
String readData;

void  setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init();  // Init RC522 
  // Keyboard.begin(); //Init keyboard
}
 
void loop() {

  //Serial.print("test\n");
  
  // Reset the loop if no new card present on the sensor/reader. This saves  the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  
  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial())
    return;
 
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  //reads the tag's UID number: four pairs of HEX digits (pair of HEX digits = 4+4  bits or 1 byte), output by the mfrc lib in DEC
  //converts each pair back to  HEX, prints the whole UID at the serial (so it can be learned to be inserted in  the read tags array) and attributes it to the readData string
  for (int i =  0; i < rfid.uid.size; i++) {
    String hexKey = "0123456789ABCDEF"; // key  for the DEC to HEX conversion
    int working = int(rfid.uid.uidByte[i]); //  auxiliary variable for the DEC to HEX conversion
    
    Serial.print(hexKey[working/16]);//first  digit (16^1)
    Serial.print(hexKey[working%16]);//second digit (16^0)

    readData += hexKey[working/16];//first digit (16^1)
    readData += hexKey[working%16];//second  digit (16^0)
    
    //if (i < rfid.uid.size - 1) {//won't add a space after  the last byte 
    //  Serial.print(" ");
    //  readData += " "; 
    //}
  }

  Serial.println();

  // checks if the read tag UID (readData)  is in the known tags array
  // if so, tells the keyboard to print the value  with the same index in the passwords array
  // for (int i = 0; i < listSize; i++)  {
  //   if (readData == tags[i]) {
  //     Keyboard.print(pwds[i]);
  //     break;
  //   }
  // }

  // empties the UID-storing string for the next reading
  readData = "";
 
  rfid.PICC_HaltA(); // Halt PICC
}
