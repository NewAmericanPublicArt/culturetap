#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"
#include <Wire.h>


SdReader card;    // This object holds the information for the SDcard
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play

WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

#define DEBOUNCE 100  // button debouncer

// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 


// this sets the analog pin A0 as the input for the day/night switch
int lightPin = A0;
// this sets the threshold of darkness, as read by the photoresistor circuit with a 10k pulldown
const int lightValue = 850;

// this is the initialization of the SD card
void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

void setup() {
  /////-- I2C --////
  
  Wire.begin(1);                // join i2c bus with address #1
  Wire.onReceive(receiveEvent); // register event
  
  //////////////////
  
  // set up serial port
  Serial.begin(115200);
  putstring_nl("WaveHC with 6 buttons");
  
   putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
 
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Ready!");
}

void loop() {
  delay(100);
}

//////////-- FUNCTIONS --////////////

//-- I2C --///

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);  // print the integer
  //playfile("DO.WAV");
  //if(analogRead(lightPin) > lightValue)
  //{
    playDatabaseFile(x);
  //}

}

//////////////
// - Audio Database -- //
/*
Organized by first and last digit. Example 11,12,13 these are three story clips told by one
person. 21,22,23 are three more. Rob is num 1 so his stories are 11, 12, and 13.

*/

void playDatabaseFile(int audioNum){
 putstring(".");            // uncomment this to see if the loop isnt running
  switch (audioNum) {
    case 01:
      playfile("MI.WAV");
      break;
    case 02:
      playfile("SO.WAV");
      break;
    case 03:
      playfile("FA.WAV");
      break;
    //-- Rob R --//
    case 11:
      playfile("DO.WAV");
      break;
    case 12:
      playfile("FA.WAV");
      break;
    case 13:
      playfile("LA.WAV");
      break;
    //-- End Rob R--//  
      
    case 21:
      playfile("MI.WAV");
      break;
    case 22:
      playfile("RE.WAV");
      break;
    case 23:
      playfile("SO.WAV");
      break;
    case 31:
      playfile("SO.WAV");
      break;
    case 32:
      playfile("RE.WAV");
      break;
    case 33:
      playfile("MI.WAV");
      break;
    case 41:
      playfile("TI.WAV");
      break;
    case 42:
      playfile("SO.WAV");
      break;
    case 43:
      playfile("FA.WAV");
      break;
    case 51:
      playfile("SO.WAV");
      break;
    case 52:
      playfile("DO.WAV");
      break;
    case 53:
      playfile("TI.WAV");
      break;
    case 61:
      playfile("TI.WAV");
      break;
    case 62:
      playfile("SO.WAV");
      break;
    case 63:
      playfile("FA.WAV");
      break;  
    case 71:
      playfile("RE.WAV");
      break;
    case 72:
      playfile("TI.WAV");
      break;
    case 73:
      playfile("LA.WAV");
      break;  
    case 81:
      playfile("DO.WAV");
      break;
    case 82:
      playfile("TI.WAV");
      break;
    case 83:
      playfile("SO.WAV");
      break; 
    case 91:
      playfile("DO.WAV");
      break;
    case 92:
      playfile("LA.WAV");
      break;
    case 93:
      playfile("RE.WAV");
      break;   
      
    
  } 
}


// Plays a full file from beginning to end with no pause.
/// -- playcomplete seems to be broken don't know why -- // 
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {
  // do nothing while its playing
  }
  // now its done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}
