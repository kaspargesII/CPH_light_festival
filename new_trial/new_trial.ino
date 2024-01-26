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
int speed = 4;
int spacing = 0; 
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
uint8_t paletteIndex = 0;

CRGBPalette16 purplePalette = CRGBPalette16 (
    CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::DarkViolet,
    
    CRGB::Magenta,
    CRGB::Magenta,
    CRGB::Linen,
    CRGB::Linen,
    
    CRGB::Magenta,
    CRGB::Magenta,
    CRGB::DarkViolet,
    CRGB::DarkViolet,

    CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::Linen,
    CRGB::Linen
);


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
      leds[i] = CRGB (255,0,0);
      leds2[i] = CRGB (255,0,0);
      FastLED.show();
      delay(30);
    }
  }
  EVERY_N_MILLISECONDS(60000){
    speed = 12;
  }
  EVERY_N_MILLISECONDS(18000){
    spacing = rand() % 800;
  }


  uint8_t sawPos = map(beat8(speed,spacing),0,255,0,NUM_LEDS-1);
  uint8_t sawPos2 = map(beat8(speed,speed*400 + spacing*0.35),0,255,0,NUM_LEDS-1);
  uint8_t sawPos3 = map(beat8(speed,speed*800 + spacing*0.6),0,255,0,NUM_LEDS-1);
  uint8_t sawPos4 = map(beat8(speed,speed*1200 + spacing*0.2),0,255,0,NUM_LEDS-1);
  uint8_t sawPos5 = map(beat8(speed,speed*1800 + spacing*0.8),0,255,0,NUM_LEDS-1);
  uint8_t sawPos6 = map(beat8(speed,speed*2300 + spacing*0.67),0,255,0,NUM_LEDS-1);
  uint8_t sawPos7 = map(beat8(speed,speed*2700 + spacing*0.9),0,255,0,NUM_LEDS-1);
  uint8_t sawPos8 = map(beat8(speed,speed*3500 + spacing*0.12),0,255,0,NUM_LEDS-1);
  uint8_t sawPos9 = map(beat8(speed,speed*4200 + spacing*0.45),0,255,0,NUM_LEDS-1);

  uint8_t wsawPos = map(beat8(speed,spacing+1000),0,255,0,NUM_LEDS-1);
  uint8_t wsawPos2 = map(beat8(speed,speed*600 + spacing*0.35),0,255,0,NUM_LEDS-1);
  uint8_t wsawPos3 = map(beat8(speed,speed*900 + spacing*0.6),0,255,0,NUM_LEDS-1);
  uint8_t wsawPos4 = map(beat8(speed,speed*1400 + spacing*0.2),0,255,0,NUM_LEDS-1);
  uint8_t wsawPos5 = map(beat8(speed,speed*1800 + spacing*0.8),0,255,0,NUM_LEDS-1);
  uint8_t wsawPos6 = map(beat8(speed,speed*2800 + spacing*0.67),0,255,0,NUM_LEDS-1);
  uint8_t wsawPos7 = map(beat8(speed,speed*3000 + spacing*0.9),0,255,0,NUM_LEDS-1);
  uint8_t wsawPos8 = map(beat8(speed,speed*4000 + spacing*0.12),0,255,0,NUM_LEDS-1);
  uint8_t wsawPos9 = map(beat8(speed,speed*4500 + spacing*0.45),0,255,0,NUM_LEDS-1);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,40));
  fill_solid(leds2, NUM_LEDS, CRGB(0,0,40));
  //uint8_t sawPos2 = map(beat8(12,0),0,255,0,NUM_LEDS-4);
  //uint8_t sinBeat2  = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
  //uint8_t sinBeat3  = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);

  // If you notice that your pattern is missing out certain LEDs, you
  // will need to use the higher resolution beatsin16 instead. In this
  // case remove the 3 lines above and replace them with the following:
  //uint16_t sinBeat   = beatsin16(10, 0, NUM_LEDS - 1, 0, 0);
  //uint16_t sinBeat2  = beatsin16(30, 0, NUM_LEDS - 1, 0, 60);
  // uint16_t sinBeat3  = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);
  
  leds[sawPos]   = CRGB(10,255,12);
  leds[sawPos2]   = CRGB(10,255,12);
  leds[sawPos3]   = CRGB(10,255,12);
  leds[sawPos4]   = CRGB(10,255,12);
  leds[sawPos5]   = CRGB(10,255,12);
  leds[sawPos6]   = CRGB(10,255,12);
  leds[sawPos7]   = CRGB(10,255,12);
  leds[sawPos8]   = CRGB(10,255,12);
  leds[sawPos9]   = CRGB(10,255,12);

  leds2[wsawPos]   = CRGB(10,255,12);
  leds2[wsawPos2]   = CRGB(10,255,12);
  leds2[wsawPos3]   = CRGB(10,255,12);
  leds2[wsawPos4]   = CRGB(10,255,12);
  leds2[wsawPos5]   = CRGB(10,255,12);
  leds2[wsawPos6]   = CRGB(10,255,12);
  leds2[wsawPos7]   = CRGB(10,255,12);
  leds2[wsawPos8]   = CRGB(10,255,12);
  leds2[wsawPos9]   = CRGB(10,255,12);
  //leds[70]   = CRGB::Green;
  //leds[sawPos2+7] = CRGB::Red;
  //leds[sinBeat2]  = CRGB::Blue;
  //leds[sinBeat3]  = CRGB::Red;
  EVERY_N_MILLISECONDS(5){
    for(int i = 0; i < 4; i++) {
      blur1d(leds, NUM_LEDS, 40);
      blur1d(leds2, NUM_LEDS, 40);
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
