
#if VS1053

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#include "stateEnt/virtual/lightShowBase/lightShowBase.h"

/*
  -DVS1053_SCLK_PIN=18
  -DVS1053_MISO_PIN=19
  -DVS1053_MOSI_PIN=23
  -DVS1053_CS_PIN=5
  -DVS1053_RST_PIN=22
  -DVS1053_XDCS_PIN=21
  -DVS1053_SDCS_PIN=17
  -DVS1053_DREQ_PIN=16
*/

// define the pins used
#define CLK VS1053_SCLK_PIN  // (18) // SPI Clock, shared with SD card
#define MISO VS1053_MISO_PIN // (19) // Input data, from VS1053/SD card
#define MOSI VS1053_MOSI_PIN // (23) // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins.
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET VS1053_RST_PIN // (22) // VS1053 reset pin (output)
#define BREAKOUT_CS VS1053_CS_PIN     // (5)  // VS1053 chip select pin (output)
#define BREAKOUT_DCS VS1053_XDCS_PIN  // (21) // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
// #define SHIELD_RESET -1 // VS1053 reset pin (unused!)
// #define SHIELD_CS 7     // VS1053 chip select pin (output)
// #define SHIELD_DCS 6    // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS VS1053_SDCS_PIN // (17) // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ VS1053_DREQ_PIN // (16) // VS1053 Data request, ideally an Interrupt pin

// static Adafruit_VS1053_FilePlayer musicPlayer =
// create breakout-example object!
// Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
// create shield-example object!
// Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
static Adafruit_VS1053_FilePlayer musicPlayer =
    // create breakout-example object!
    Adafruit_VS1053_FilePlayer(MOSI, MISO, CLK, BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

class JSAudio : public LightShowBase
{
public:
  /// File listing helper
  void printDirectory(File dir, int numTabs)
  {
    while (true)
    {

      File entry = dir.openNextFile();
      if (!entry)
      {
        // no more files
        // Serial.println("**nomorefiles**");
        break;
      }
      for (uint8_t i = 0; i < numTabs; i++)
      {
        Serial.print('\t');
      }
      Serial.print(entry.name());
      if (entry.isDirectory())
      {
        Serial.println("/");
        printDirectory(entry, numTabs + 1);
      }
      else
      {
        // files have sizes, directories do not
        Serial.print("\t\t");
        Serial.println(entry.size(), DEC);
      }
      entry.close();
    }
  }

  void setup()
  {
    if (!musicPlayer.begin())
    { // initialise the music player
      Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
      while (1)
        ;
    }
    Serial.println(F("VS1053 found"));

    if (!SD.begin(CARDCS))
    {
      Serial.println(F("CRITICAL: SD failed, or not present"));
      // while (1)
      // ; // don't do anything more
    }

    // list files
    printDirectory(SD.open("/"), 0);

    // Set volume for left, right channels. lower numbers == louder volume!
    musicPlayer.setVolume(20, 20);

    // Timer interrupts are not suggested, better to use DREQ interrupt!
    // musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

    // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
    // audio playing
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT); // DREQ int

    // Play one file, don't return until complete
    Serial.println(F("Playing track 001"));
    musicPlayer.playFullFile("/track001.mp3");
    // Play another file in the background, REQUIRES interrupts!
    // Serial.println(F("Playing track 002"));
    // musicPlayer.startPlayingFile("/track002.mp3");
  }

  void pausePlaying(bool p)
  {
    Serial.print("Audio pause: ");
    Serial.println(p);
    musicPlayer.pausePlaying(p);
  }

  void stopPlaying()
  {
    Serial.println("Audio stop");
    musicPlayer.stopPlaying();
  }
};

#endif