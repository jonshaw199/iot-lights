
#if false // JSVS1053

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

// This ESP_VS1053_Library
#include <VS1053.h>

// Please find SampleMp3.h file here:
//   github.com/baldram/ESP_VS1053_Library/blob/master/examples/Mp3PlayerDemo/SampleMp3.h
#include "SampleMp3.h"
#include "stateEnt/virtual/lightShowBase/lightShowBase.h"

// Wiring of VS1053 board (SPI connected in a standard way)
#define VS1053_CS VS1053_CS_PIN
#define VS1053_DCS VS1053_XDCS_PIN
#define VS1053_DREQ VS1053_DREQ_PIN

#define VOLUME 50 // volume level 0-100

static VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);

class JSAudio : public LightShowBase
{
public:
  void pausePlaying(bool p)
  {
    Serial.print("Audio pause: ");
    Serial.println(p);
    // musicPlayer.pausePlaying(p);
  }

  void stopPlaying()
  {
    Serial.println("Audio stop");
    // musicPlayer.stopPlaying();
  }

  String getName()
  {
    return "STATE_AUDIO";
  }

  void preStateChange(int s)
  {
    // musicPlayer.stopPlaying();
    // musicPlayer.reset();
  }

  void setup()
  {
    // initialize SPI
    SPI.begin();

    Serial.println("Hello VS1053!\n");
    // initialize a player
    player.begin();
    Serial.print("Chip version: ");
    Serial.println(player.getChipVersion());
    if (player.getChipVersion() == 4)
    { // Only perform an update if we really are using a VS1053, not. eg. VS1003
      player.loadDefaultVs1053Patches();
    }
    player.switchToMp3Mode(); // optional, some boards require this
    player.setVolume(VOLUME);
  }

  void loop()
  {
    // Serial.println("Playing sound... ");
    Serial.print("s");

    // play mp3 flow each 3s
    player.playChunk(sampleMp3, sizeof(sampleMp3));
    delay(3000);
  }
};

#endif