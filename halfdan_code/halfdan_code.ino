

#include <FastLED.h>

#include "HX711.h"

 

#define rows 12

#define columns 100

#define LOADCELL_DOUT_PIN 4

#define LOADCELL_SCK_PIN 13

 

int timer_intervals[]{ 40, 30, 50, 30, 40, 50, 30, 50, 30, 50, 30, 50 };

unsigned long timers[]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int rain_positions[12][100];

bool dir = false;

CRGB leds[rows][columns];

int space_to_next_drop = 5;

float weight;

float weightTranslated;

int resetCount = 0;

unsigned long resetTimer = 0;

HX711 scale;

int loopCount = 0;

unsigned long lastloopCountTime = 0;

unsigned long weightPrintTimer = 0;

int jumpState = 0;

float jumpMax = 0;

unsigned long upMoveTimer = 0;

int upMoveDuration;

int durationMultiplier = 80; // used to impact the final time the light upmove lasts after a jump

float calibration_factor = -7050;  // -7050 worked for my 440lb max scale setup

int weightMaxCount = 0;

 

void setup() {

 

  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, 22>(leds[0], columns);

  FastLED.addLeds<NEOPIXEL, 23>(leds[1], columns);

  FastLED.addLeds<NEOPIXEL, 15>(leds[2], columns);

  FastLED.addLeds<NEOPIXEL, 25>(leds[3], columns);

  FastLED.addLeds<NEOPIXEL, 26>(leds[4], columns);

  FastLED.addLeds<NEOPIXEL, 27>(leds[5], columns);

  FastLED.addLeds<NEOPIXEL, 16>(leds[6], columns);

  FastLED.addLeds<NEOPIXEL, 17>(leds[7], columns);

  FastLED.addLeds<NEOPIXEL, 18>(leds[8], columns);

  FastLED.addLeds<NEOPIXEL, 19>(leds[9], columns);

  FastLED.addLeds<NEOPIXEL, 21>(leds[10], columns);

  FastLED.addLeds<NEOPIXEL, 14>(leds[11], columns);

 

  for (int c = columns - 1; c >= 0; c--) {

    Serial.println(c);

  }

  rain_positions[0][0] = 1;  // test drop

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale();

  scale.tare();                         //Reset the scale to 0

  scale.set_scale(calibration_factor);  //Adjust to this calibration factor

  Serial.println("Starting: Init scale reset");

}

 

void loop() {

  bool changes_to_make = false;

  float cute = scale.get_units();
  Serial.println("cute  ");
  Serial.println(cute);
  Serial.println("\n");
  

  for (int r = 0; r < rows; r++) {

 

    if (millis() > timers[r] + timer_intervals[r]) {

      timers[r] = millis();

      changes_to_make = true;

 

      if (dir){

       

        // move all existing drops

        for (int c = columns - 1; c >= 0; c--) {

          if (rain_positions[r][c] == 1) {

            rain_positions[r][c] = 0;                                // turn off old one

            if (c + 1 <= columns - 1) rain_positions[r][c + 1] = 1;  // turn on new one

          }

        }

 

        // add new drop in the front

        bool drop_found = false;

        for (int c = 0; c < space_to_next_drop; c++) {

          if (rain_positions[r][c] == 1) drop_found = true;

        }

        if (!drop_found) {

          rain_positions[r][0] = 1;

          space_to_next_drop = int(random(4, 10));

        }

      }

 

      else{

        // move all existing drops

        for (int c = 0; c <= columns-1; c++) {

          if (rain_positions[r][c] == 1) {

            rain_positions[r][c] = 0;                                // turn off old one

            if (c - 1 >= 0) rain_positions[r][c - 1] = 1;  // turn on new one

          }

        }

 

        // add new drop in the front

        bool drop_found = false;

        for (int c = columns - 1; c > columns - 1 - space_to_next_drop; c--) {

          if (rain_positions[r][c] == 1) drop_found = true;

        }

        if (!drop_found) {

          rain_positions[r][columns - 1] = 1;

          space_to_next_drop = int(random(4, 10));

        }

      }

 

      // send new pixels to physical strip

      for (int c = 0; c < columns; c++) {

 

        if (rain_positions[r][c] == 1) {

          leds[r][c] = CRGB(50, 50, 50);

          if (c - 1 >= 0) leds[r][c - 1] = CRGB(255, 255, 255);

          if (c - 2 >= 0) leds[r][c - 2] = CRGB(50, 50, 50);

        } else leds[r][c] = CRGB(0, 0, 0);

      }

    }

  }

 

  if (changes_to_make) {

    FastLED.show();

  }

 

  // READING THE SCALE WEIGHT - every loop

  readScale();

 

  switch (jumpState) {

 

    case 0:                          // waiting for jump

      if (weightTranslated > 0.5) {  // jump started | 0.5 being a threashold value

        jumpMax = weightTranslated;

        jumpState = 1;

        Serial.print("Jump started ");

        Serial.println(jumpMax);

      }

      break;

 

    case 1:  // finding max weight

      if (weightTranslated > jumpMax) {

        jumpMax = weightTranslated;

        weightMaxCount = 0;

      } else {

        weightMaxCount++;

        if (weightMaxCount > 3) {  // Max weight found - if no new max weight was detected for 3 rounds the jump is over

          jumpState = 2;

          Serial.print("Jump ended ");

          Serial.println(jumpMax);

 

          upMoveDuration = jumpMax * durationMultiplier; // calculate upmove time

          upMoveTimer = millis(); // load the timer

          Serial.print("upMoveduration : ");

          Serial.println(upMoveDuration);

 

          jumpMax = 0;

 

          dir = false; // change to moving lights up

        }

      }

 

      break;

 

    case 2:  // waiting for jumper to move away

      if (weightTranslated < 0.5) {

        jumpState = 0;

        Serial.println("Platform empty - start over");

      }

      break;

  }

 

  // change direction when upmove is done

  if (millis() > upMoveTimer + upMoveDuration) {

    dir = true;

  }

 

  // RESET SCALE - default value = 1000

  resetScale(1000);  // input : nr of continous readings to be off by > 0.3 before resetting

 

  // PRINT FRAME(loop) COUNT - default value = 2000

  printLoopCount(2000);  // input : nr of loops to calculate across

 

  // DEBUG PRINTS - last number default value = 1000

  if (millis() > weightPrintTimer + 1000) {  // last number is millis between prints  |  Set to 1 for printing all reading

    weightPrintTimer = millis();

    //printDebug();

  }

}

// --- HELPER FUNCTIONS ---

 

void printDebug() {

 

  Serial.print("Reading: ");

  Serial.print(weight);

  Serial.print(" ");

  Serial.print(weightTranslated);

  Serial.print(" lbs");  //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person

  Serial.print("    calibration_factor: ");

  Serial.print(calibration_factor);

  Serial.println();

}

 

void readScale() {

  weight = scale.get_units();

  // translate weight to prevent errors in the scale data

  if (weight > 700) weightTranslated = 1189.99 - weight;  // figure out how to handle this

  else if (weight > 0) weightTranslated = weight;

  else if (weight < -1000) weightTranslated = 0;  // figure out how to handle this

  else if (weight < 0) weightTranslated = weight * -1;

}

 

void printLoopCount(int nrOfLoops) {

  loopCount++;

  if (loopCount == nrOfLoops) {

    float avgMillisPerLoop = (millis() - lastloopCountTime) / loopCount;

    float loopsPerSec = 1000 / avgMillisPerLoop;

    Serial.println();

    Serial.print("Average millis for each loop: ");

    Serial.print(avgMillisPerLoop);

    Serial.print("  Loops per second: ");

    Serial.println(loopsPerSec);

    Serial.println();

    loopCount = 0;

    lastloopCountTime = millis();

  }

}

 

void resetScale(int loopsToReset) {

  if (millis() > resetTimer + 10) {  // runs every 10 millis. Due to fastest loop iteration measured being 12 millis/loop

    resetTimer = millis();

    if (abs(weight) > 0.3) {

      resetCount++;

      if (resetCount > loopsToReset) {  // reset the scale

        resetCount = 0;

        Serial.println();

        printDebug();

        scale.tare();  //Reset the scale to 0

        Serial.println("SCALE RESET");

        readScale();

        printDebug();

        Serial.println();

      }

    }

    else resetCount = 0;

  }

}