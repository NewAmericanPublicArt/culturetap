#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield
#define ADDRESS 42

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

//-- Day or Night -- //
int lightPin = A0;  //define a pin for Photo resistor
int lightValue;
int dnVal;

//-- Strings --//
String cardString;
String bigHexStr;
String hexAt0;
String hexAt1;
String hexAt2;
String hexAt3;
String hexAt4;
String hexAt5;
String hexAt6;
String hexAt7;

//-- numbers --//
uint8_t uid[] = { 
  0, 0, 0, 0, 0, 0, 0 }; 
unsigned long cardUniNum = 3000000000;
int var0 = 0;
int var1 = 0;
int var2 = 0;
int var3 = 0;
int var4 = 0;
int var5 = 0;
int var6 = 0;
int var7 = 0;
int var8 = 0;
int var9 = 0;

void setup(void) {

  Serial.begin(115200);
  Serial.println("Hello!");
  
  pinMode(lightPin, INPUT);
  int lightValue = analogRead(lightPin);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");
  Serial.println("");

  /////-- I2C --//////

  Wire.begin(); //-- no address specified, thus joins as Master
  // I2C uses pins 4 and 5 // 
  // 5 = white, 4 = yellow //

  ////////////////////

} //-- end setup


void loop(void) {
  
  //Serial.print("intheloop01");
  
  uint8_t success;
  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) 
  {
    ////////-- Determine what card does what --////////

    cardString = String(bigCardNum());
    //Serial.println(cardString.charAt(7));
    runDataBase(cardString.charAt(7));

    // Wait a bit before reading the card again
    delay(1000); 
  }

  else
  {
    Serial.println("Ooops ... authentication failed: Try another key?");
  }


}//-end void loop

//////----- FUNCTIONS -----///////


unsigned long bigCardNum(){

  bigHexStr = 
    String(uid[0],HEX) 
    +String(uid[1],HEX)
      +String(uid[2],HEX) 
        +String(uid[3],HEX);

  hexAt0 = String(bigHexStr.charAt(0));
  hexAt1 = String(bigHexStr.charAt(1));
  hexAt2 = String(bigHexStr.charAt(2));
  hexAt3 = String(bigHexStr.charAt(3));
  hexAt4 = String(bigHexStr.charAt(4));
  hexAt5 = String(bigHexStr.charAt(5));
  hexAt6 = String(bigHexStr.charAt(6));
  hexAt7 = String(bigHexStr.charAt(7));

  cardUniNum = 
    (hexToDec(hexAt7))
    +(hexToDec(hexAt6)*16)
      +(hexToDec(hexAt5)*256)
        +(hexToDec(hexAt4)*4096)
          +(hexToDec(hexAt3)*65536)
            +(hexToDec(hexAt2)*1048576)
              +(hexToDec(hexAt1)*16777216)
                +(hexToDec(hexAt0)*268435456);

  return cardUniNum;
};

///-- HEX to DEC -- I only use this for single digits - //  
unsigned int hexToDec(String hexString) {

  unsigned int decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;

    //-- Prints nice explaination --//
    /*
    Serial.print(hexString);
     Serial.print(" = ");
     Serial.print(nextInt);
     Serial.println("");
     */
  }
  return decValue;
}

//-- Day or Night --//
void dayOrNight(){
  Serial.print("...Lights if < 250... Sensor says: ");
  Serial.println(analogRead(lightPin));
  if (analogRead(lightPin) < 250){
      dnVal = 1;
  }else{
      dnVal = 2;
  }
}

//-- I2C Master Send Function --//

void i2c(int wireCom){
  
  dayOrNight(); // check the sensor
  
  if (dnVal != 1 && dnVal != 2){
    Serial.println("dnVal error! Setting dnVal = 1");
    dnVal = 1;
  }
  
  Wire.beginTransmission(dnVal); // transmit to device -- set device as # in slave pde
  Wire.write(wireCom);        // sends bytes
  Wire.endTransmission();    // stop transmitting
}
/////////////////////////////////


//-- DATABASE Switch --//    
void runDataBase(char singleChar){

  switch (singleChar) {
  case '0': 
    switch (var0) {
    case 0:
      i2c(01);
      Serial.print("var0 = ");
      Serial.println(var0);
      var0 = 1;
      break;
    case 1:
      i2c(02);
      Serial.print("var0 = ");
      Serial.println(var0);
      var0 = 2;
      break;
    case 2:
      i2c(03);
      Serial.print("var0 = ");
      Serial.println(var0);
      var0 = 0;
      break;//- var
    }//- end var
    break;//- #
  case '1': 
    switch (var1) {
    case 0:
      i2c(11);
      Serial.print("var1 = ");
      Serial.println(var1);
      var1 = 1;
      break;
    case 1:
      i2c(12);
      Serial.print("var1 = ");
      Serial.println(var1);
      var1 = 2;
      break;
    case 2:
      //send Ard2 message play audio var02
      i2c(13);
      Serial.print("var1 = ");
      Serial.println(var1);
      var1 = 0;
      break;
    }//- end var
    break;//- #
  case '2': 
    switch (var2) {
    case 0:
      i2c(21);
      Serial.print("var2 = ");
      Serial.println(var2);
      var2 = 1;
      break;
    case 1:
      i2c(22);
      Serial.print("var2 = ");
      Serial.println(var2);
      var2 = 2;
      break;
    case 2:
      i2c(23);
      Serial.print("var2 = ");
      Serial.println(var2);
      var2 = 0;
      break;//- var
    }//- end var
    break;//- #
  case '3': 
    switch (var3) {
    case 0:
      i2c(31);
      Serial.print("var3 = ");
      Serial.println(var3);
      var3 = 1;
      break;
    case 1:
      i2c(32);
      Serial.print("var3 = ");
      Serial.println(var3);
      var3 = 2;
      break;
    case 2:
      i2c(33);
      Serial.print("var3 = ");
      Serial.println(var3);
      var3 = 0;
      break;//- var
    }//- end var
    break;//- #
  case '4': 
    switch (var4) {
    case 0:
      i2c(41);
      Serial.print("var4 = ");
      Serial.println(var4);
      var4 = 1;
      break;
    case 1:
      i2c(42);
      Serial.print("var4 = ");
      Serial.println(var4);
      var4 = 2;
      break;
    case 2:
      i2c(43);
      Serial.print("var4 = ");
      Serial.println(var4);
      var4 = 0;
      break;//- var
    }//- end var
    break;//- #
  case '5': 
    switch (var5) {
    case 0:
      i2c(51);
      Serial.print("var5 = ");
      Serial.println(var5);
      var5 = 1;
      break;
    case 1:
      i2c(52);
      Serial.print("var5 = ");
      Serial.println(var5);
      var5 = 2;
      break;
    case 2:
      i2c(53);
      Serial.print("var5 = ");
      Serial.println(var5);
      var5 = 0;
      break;//- var
    }//- end var
    break;//- #
  case '6': 
    switch (var6) {
    case 0:
      i2c(61);
      Serial.print("var6 = ");
      Serial.println(var6);
      var6 = 1;
      break;
    case 1:
      //send Ard2 message play audio var01
      i2c(62);
      Serial.print("var6 = ");
      Serial.println(var6);
      var6 = 2;
      break;
    case 2:
      i2c(63);
      Serial.print("var6 = ");
      Serial.println(var6);
      var6 = 0;
      break;//- var
    }//- end var
    break;//- #
  case '7': 
    switch (var7) {
    case 0:
      i2c(71);
      Serial.print("var7 = ");
      Serial.println(var7);
      var7 = 1;
      break;
    case 1:
      i2c(72);
      Serial.print("var7 = ");
      Serial.println(var7);
      var7 = 2;
      break;
    case 2:
      i2c(73);
      Serial.print("var7 = ");
      Serial.println(var7);
      var7 = 0;
      break;//- var
    }//- end var
    break;//- #
  case '8': 
    switch (var8) {
    case 0:
      i2c(81);
      Serial.print("var8 = ");
      Serial.println(var8);
      var8 = 1;
      break;
    case 1:
      i2c(82);
      Serial.print("var8 = ");
      Serial.println(var8);
      var8 = 2;
      break;
    case 2:
      i2c(83);
      Serial.print("var8 = ");
      Serial.println(var8);
      var8 = 0;
      break;//- var
    }//- end var
    break;//- #
  case '9': 
    switch (var9) {
    case 0:
      i2c(91);
      Serial.print("var9 = ");
      Serial.println(var9);
      var9 = 1;
      break;
    case 1:
      i2c(92);
      Serial.print("var9 = ");
      Serial.println(var9);
      var9 = 2;
      break;
    case 2:
      i2c(93);
      Serial.print("var9 = ");
      Serial.println(var9);
      var9 = 0;
      break;//- var
    }//- end var
    break;//- #
  }//- end singleChar
}//- end database





