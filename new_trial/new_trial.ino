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
int patternMode =0;
int patternMode2 = 0;
int illusion = 0;
//CRGB LEDS[TotalLeds];
uint8_t paletteIndex = 0;


// Palettes!!

DEFINE_GRADIENT_PALETTE (heatmap_gp) {
    0,   0,   0,   0,   //black
  128, 255,   0,   0,   //red
  200, 255, 255,   0,   //bright yellow
  255, 255, 255, 255    //full white 
};

DEFINE_GRADIENT_PALETTE( rainbow_gp ) {
    0, 126,  1,142,
   25, 171,  1, 26,
   48, 224,  9,  1,
   71, 237,138,  1,
   94,  52,173,  1,
  117,   1,201,  1,
  140,   1,211, 54,
  163,   1,124,168,
  186,   1,  8,149,
  209,  12,  1,151,
  232,  12,  1,151,
  255, 171,  1,190};


DEFINE_GRADIENT_PALETTE( purplefly_gp ) {
    0,   0,  0,  0,
   63, 239,  0,122,
  191, 252,255, 78,
  255,   0,  0,  0};


DEFINE_GRADIENT_PALETTE( bhw2_39_gp ) {
    0,   2,184,188,
   33,  56, 27,162,
   66,  56, 27,162,
  122, 255,255, 45,
  150, 227, 65,  6,
  201,  67, 13, 27,
  255,  16,  1, 53};

DEFINE_GRADIENT_PALETTE( bhw1_01_gp ) {
    0, 227,101,  3,
  117, 194, 18, 19,
  255,  92,  8,192};


DEFINE_GRADIENT_PALETTE( teabearrose_gp ) {
    0, 107,  1,  5,
   25, 165, 25, 45,
   38, 184, 82, 88,
   63, 229,133,130,
   89, 229,133,130,
  109, 186, 40,  4,
  117, 215,139, 96,
  122, 148,  8,  1,
  127, 215,139, 96,
  132, 148,  8,  1,
  137, 215,139, 96,
  145, 186, 40,  4,
  165, 229,133,130,
  191, 229,133,130,
  216, 184, 82, 88,
  229, 165, 25, 45,
  255, 107,  1,  5};


DEFINE_GRADIENT_PALETTE( Need_I_Say_More_gp ) {
    0, 224, 49, 13,
   42, 224, 49, 13,
   84, 232, 73, 22,
   99, 239,103, 34,
  114, 234,136, 50,
  126, 229,176, 68,
  137, 144,159, 75,
  153,  82,142, 83,
  168,  48,127, 85,
  211,  25,111, 89,
  255,  25,111, 89};



CRGBPalette16 myPal = heatmap_gp;
CRGBPalette16 myPal2 = rainbow_gp;
CRGBPalette16 myPal3 = purplefly_gp;
CRGBPalette16 myPal4 = bhw2_39_gp;
CRGBPalette16 myPal5 = bhw1_01_gp;
CRGBPalette16 myPal6 = teabearrose_gp;
CRGBPalette16 myPal7 = Need_I_Say_More_gp;


CRGBPalette16 palletes[] = {myPal,myPal2,myPal3,myPal4,myPal5,myPal6,myPal7};


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
    patternMode = rand()%7;
    patternMode2 = rand()%7;
    for(uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds2[i] = ColorFromPalette(palletes[patternMode2], i*2.5);
      FastLED.show();
      delay(30);
    }
    //patternMode++; 
    //if (patternMode > 7){
    //  patternMode = 0;
    //  }
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
