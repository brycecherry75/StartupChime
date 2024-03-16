// Startup chime generator for Raspberry Pi Pico by Bryce Cherry

#define SIMPLE_RC_FILTER // comment out if using I2S DAC

#include <PinsToBus.h> // obtain at http://github.com/brycecherry75/PinsToBus
#if not defined SIMPLE_RC_FILTER
#include <I2S.h>
I2S i2s(OUTPUT);
#else
#include <PWMAudio.h>
PWMAudio pwm(22); // Pin 22
#endif

// GPIO pin numbers
#define pBCLK 20
#define pWS (pBCLK+1)
#define pDOUT 22 // connect to an amplifier via an RC low pass filter (1K0/22 nF) and couple via 10 uF (+ve side of capacitor to 1K0/22 nF junction and -ve side to Volume control (10K))

const byte ChimeSelectionPinCount = 4;
const byte ChimeSelectionPins[ChimeSelectionPinCount] PROGMEM = {0, 1, 2, 3}; // +1, +2, +4, +8 - DIP or rotary hexadecimal switch Common to GND via 1K0
const byte PlayPin = 5;

const word sampleRate = 16000; // for all sounds - minimum supported by I2S interface and provides a good quality/memory usage ratio
const byte ChimesInstalled = 20; // the maximum chimes chosen which can fit into program memory

/*

  Data for sounds which would be good as a startup chime; all are converted with WAVoperations obtainable at http://github.com/brycecherry75/WAVoperations
  Due to copyright, no waveform data is included (waveform data files require the following format - "name" is to identify the chime):

  const unsigned long nameChimeSampleRate = 16000;
  const unsigned long nameChimeLength = 48164;
  const word nameChime[nameChimeLength] PROGMEM = {data_array_in_hexadecimal};

*/

#include "adultswimchime.h" // before Rick and Morty
#include "amazonprimevideochime.h" // streaming service
#include "bentochime.h" // in credits of HouseBroken animated series
#include "cbschime.h"
#include "datamchime.h" // from the SNES/SFC game Keeper (first sound played after reset)
#include "earthsparkchime.h" // Transformers Earthspark (title screen)
#include "hbochime.h"
#include "huluchime.h" // streaming service - in credits
#include "maxchime.h" // streaming service
#include "netflixchime.h" // streaming service
#include "nickchime.h" // Nickelodeon
#include "peacockchime.h" // streaming service
// synthesized tunes in Farzar - this show also has a synthesized tune which would be good as a death chime
#include "farzarchime1.h"
#include "farzarchime2.h"
#include "farzarchime3.h"
#include "farzarchime5.h"
#include "farzarchime6.h"
#include "farzarchime7.h"
#include "farzarchime8.h"
#include "farzarchime9.h"
#include "farzarchime13.h"

void Depop(word FirstSample) {
  if (FirstSample > 0x8000) {
    FirstSample ^= 0xFFFF;
    FirstSample++;
    word DepopSampleValue = 0;
    for (word DepopSample = 0; DepopSample < FirstSample; DepopSample++) {
      DepopSampleValue--;
      while (!pwm.availableForWrite()) {
      }
      pwm.write(DepopSampleValue);
    }
  }
  else {
    for (word DepopSample = 0; DepopSample < FirstSample; DepopSample++) {
      while (!pwm.availableForWrite()) {
      }
      pwm.write(DepopSample);
    }
  }
}

void setup() {
  PinsToBus.readByte(ChimeSelectionPins, ChimeSelectionPinCount, true, true);
  pinMode(PlayPin, INPUT_PULLUP);
#if not defined SIMPLE_RC_FILTER
  i2s.setDATA(pDOUT); // we are using a simple RC filter for low cost and does not require high quality audio
  i2s.setBCLK(pBCLK); // we are using a simple RC filter for low cost and does not require high quality audio
  i2s.setBitsPerSample(16);
  i2s.begin(sampleRate);
#else
  pwm.begin(sampleRate);
#endif
}

void loop() {
  if (digitalRead(PlayPin) == HIGH) {
    byte ChimeToPlay = PinsToBus.readByte(ChimeSelectionPins, ChimeSelectionPinCount, false, true);
    ChimeToPlay ^= ((1 << ChimeSelectionPinCount) - 1); // when a DIP switch is ON, input will be LOW and therefore requires inversion
    if (ChimeToPlay == 0 || ChimeToPlay > ChimesInstalled) { // select a random chime if DIP switch combination is 0 or above ChimesInstalled
      delayMicroseconds((analogReadTemp() * 10));
      randomSeed(micros() + (analogReadTemp() * 100));
      ChimeToPlay = random(1, (ChimesInstalled + 1));
    }
    word WaveformPointRead;
    switch (ChimeToPlay) {
      case 1:
        for (unsigned long WaveformPoint = 0; WaveformPoint < AdultSwimChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(AdultSwimChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 2:
        for (unsigned long WaveformPoint = 0; WaveformPoint < AmazonPrimeVideoChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(AmazonPrimeVideoChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 3:
        for (unsigned long WaveformPoint = 0; WaveformPoint < BentoChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(BentoChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
        }
        break;
      case 4:
        for (unsigned long WaveformPoint = 0; WaveformPoint < CBSchimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(CBSchime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 5:
        for (unsigned long WaveformPoint = 0; WaveformPoint < DatamChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(DatamChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 6:
        for (unsigned long WaveformPoint = 0; WaveformPoint < EarthsparkChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(EarthsparkChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 7:
        for (unsigned long WaveformPoint = 0; WaveformPoint < HBOchimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(HBOchime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 8:
        for (unsigned long WaveformPoint = 0; WaveformPoint < HuluChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(HuluChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 9:
        for (unsigned long WaveformPoint = 0; WaveformPoint < MaxChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(MaxChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 10:
        for (unsigned long WaveformPoint = 0; WaveformPoint < NetflixChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(NetflixChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 11:
        for (unsigned long WaveformPoint = 0; WaveformPoint < NetflixChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(NickChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 12:
        for (unsigned long WaveformPoint = 0; WaveformPoint < PeacockChimeLength; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(PeacockChime + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 13:
        for (unsigned long WaveformPoint = 0; WaveformPoint < FarzarChime_1_Length; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(FarzarChime_1 + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 14:
        for (unsigned long WaveformPoint = 0; WaveformPoint < FarzarChime_2_Length; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(FarzarChime_2 + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 15:
        for (unsigned long WaveformPoint = 0; WaveformPoint < FarzarChime_3_Length; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(FarzarChime_3 + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 16:
        for (unsigned long WaveformPoint = 0; WaveformPoint < FarzarChime_5_Length; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(FarzarChime_5 + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 17:
        for (unsigned long WaveformPoint = 0; WaveformPoint < FarzarChime_6_Length; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(FarzarChime_6 + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 18:
        for (unsigned long WaveformPoint = 0; WaveformPoint < FarzarChime_7_Length; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(FarzarChime_7 + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 19:
        for (unsigned long WaveformPoint = 0; WaveformPoint < FarzarChime_8_Length; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(FarzarChime_8 + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
      case 20:
        for (unsigned long WaveformPoint = 0; WaveformPoint < FarzarChime_13_Length; WaveformPoint++) {
          WaveformPointRead = pgm_read_word_near(FarzarChime_13 + WaveformPoint);
#if not defined SIMPLE_RC_FILTER
          i2s.write16(WaveformPointRead, WaveformPointRead);
#else
          if (WaveformPoint == 0) {
            Depop(WaveformPointRead);
          }
          while (!pwm.availableForWrite()) {
          }
          pwm.write(WaveformPointRead);
#endif
        }
        break;
    }
    Depop(WaveformPointRead);
    while (digitalRead(PlayPin) == HIGH) {
    }
  }
}
