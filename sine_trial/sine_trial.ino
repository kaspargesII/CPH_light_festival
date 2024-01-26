/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino
#include <iostream>
using namespace std;
#include <FastLED.h>
#include "HX711.h"
#define LOADCELL_DOUT_PIN  16
#define LOADCELL_SCK_PIN  4
// How many leds in your strip?
#define NUM_LEDS 100
#define COLOR_ORDER BRG
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN_ONE 23
#define DATA_PIN_TWO 22

HX711 scale;
float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup

//#define CLOCK_PIN 13
// Time variables

unsigned long timer = 0;
int interval = 200;
unsigned long fade_timer = 0;
int fade_interval = 50;
int standing_weight_interval = 50;
// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];
int rain_array1[NUM_LEDS];
int rain_array2[NUM_LEDS];

//CRGB LEDS[TotalLeds];

void update_rain(int rain_array[]){
  for( int i = NUM_LEDS-1; i >= 0; i--){
    if (rain_array[i]==1){
      rain_array[i] = 0;
      if (i < NUM_LEDS-1){
        rain_array[i+1]=1;
    }
  }
  }
}

void show_rain(int rain_array[], CRGB led_strip[]){
  for( int i = 0; i < NUM_LEDS; i++){
    if (rain_array[i]==1){
      led_strip[i] = CRGB(220,220,220);
    }
    else{
      led_strip[i] = CRGB(10,0,10);
    }
  }
  int sum = 0;
  for (int i =0; i< random(5,10);i++){
    sum = sum + rain_array[i];
    }
  if (sum == 0){
    rain_array[0] = 1;
   }
  }

void setup() { 
    // LED setup
    FastLED.addLeds<WS2811, DATA_PIN_ONE, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<WS2811, DATA_PIN_TWO, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );
    //FastLED.setBrightness(50);
    // Sensor setup
    Serial.begin(9600);
    Serial.println("HX711 calibration sketch");
    Serial.println("Remove all weight from scale");
    Serial.println("After readings begin, place known weight on scale");
    Serial.println("Press + or a to increase calibration factor");
    Serial.println("Press - or z to decrease calibration factor");

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale();
    scale.tare(); //Reset the scale to 0

    long zero_factor = scale.read_average(); //Get a baseline reading
    Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);
}


void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading:");
  float force = scale.get_units();
  Serial.print("\n");
  Serial.print(scale.get_units(), 1);
  if (scale.get_units() > 10){
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] == CRGB (255,0,0);
    }
  }

  uint8_t sineBeat =  beatsin8(3, 0, NUM_LEDS - 1, 0, 0);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,5));
  //uint8_t sawPos2 = map(beat8(12,0),0,255,0,NUM_LEDS-4);
  //uint8_t sinBeat2  = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
  //uint8_t sinBeat3  = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);

  // If you notice that your pattern is missing out certain LEDs, you
  // will need to use the higher resolution beatsin16 instead. In this
  // case remove the 3 lines above and replace them with the following:
  //uint16_t sinBeat   = beatsin16(10, 0, NUM_LEDS - 1, 0, 0);
  //uint16_t sinBeat2  = beatsin16(30, 0, NUM_LEDS - 1, 0, 60);
  // uint16_t sinBeat3  = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);

  leds[sineBeat]   = CRGB(10,255,12);
  //leds[70]   = CRGB::Green;
  //leds[sawPos2+7] = CRGB::Red;
  //leds[sinBeat2]  = CRGB::Blue;
  //leds[sinBeat3]  = CRGB::Red;
  EVERY_N_MILLISECONDS(5){
    for(int i = 0; i < 12; i++) {
      blur1d(leds, NUM_LEDS, 30);
      //blur1d(leds, sawPos, 130);
      //blur1d(leds, sawPos3, 130);
    }
  }

  //EVERY_N_MILLISECONDS(5){
  //  for(int i = 0; i < 15; i++) {
  //    blur1d(leds, NUM_LEDS, 50);
  //    //blur1d(leds, NUM_LEDS, 50);
  //  }
  //}
  
  //fadeToBlackBy(leds, NUM_LEDS, 10);

  FastLED.show();
}
