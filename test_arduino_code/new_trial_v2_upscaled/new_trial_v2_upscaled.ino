/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino
#include "HX711.h"
#include <iostream>
using namespace std;
#include <FastLED.h>
HX711 scale;
#define LOADCELL_DOUT_PIN  4
#define LOADCELL_SCK_PIN  13
// How many leds in your strip?
#define NUM_LEDS 100
#define COLOR_ORDER BRG

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI

// CHANGE
#define DATA_PIN_ONE 22
#define DATA_PIN_TWO 2
#define DATA_PIN_ONE 24
#define DATA_PIN_TWO 25
#define DATA_PIN_ONE 26
#define DATA_PIN_TWO 22
#define DATA_PIN_ONE 23
#define DATA_PIN_TWO 22
#define DATA_PIN_ONE 23
#define DATA_PIN_TWO 22
#define DATA_PIN_ONE 23
#define DATA_PIN_TWO 22

float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup
int speed_array[] = {15};
int speed_index = 0;
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
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];
CRGB leds5[NUM_LEDS];
CRGB leds6[NUM_LEDS];
CRGB leds7[NUM_LEDS];
CRGB leds8[NUM_LEDS];
CRGB leds9[NUM_LEDS];
CRGB leds10[NUM_LEDS];
CRGB leds11[NUM_LEDS];
CRGB leds12[NUM_LEDS];

// Rain position array
int rain_array1[NUM_LEDS];
int rain_array2[NUM_LEDS];
int rain_array3[NUM_LEDS];
int rain_array4[NUM_LEDS];
int rain_array5[NUM_LEDS];
int rain_array6[NUM_LEDS];
int rain_array7[NUM_LEDS];
int rain_array8[NUM_LEDS];
int rain_array9[NUM_LEDS];
int rain_array10[NUM_LEDS];
int rain_array11[NUM_LEDS];
int rain_array12[NUM_LEDS];


int patternMode =0;
uint8_t patternCounter = 0;
uint8_t paletteIndex = 0;

uint16_t brightnessScale = 150;
uint16_t indexScale = 100;
float force2 = 0;
// Pattern specific thingy
CRGB background[NUM_LEDS];

// Palette definitions from cptcity

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


DEFINE_GRADIENT_PALETTE( aurora_gp ) {
    0, 120, 30, 28,
   51, 120, 30, 28,
   51, 150, 62, 32,
  102, 150, 62, 32,
  102, 206,154, 55,
  153, 206,154, 55,
  153,  79,133, 56,
  204,  79,133, 56,
  204, 103, 70, 96,
  255, 103, 70, 96};

DEFINE_GRADIENT_PALETTE( nrwc_gp ) {
    0,   1,  1,  1,
   25,   4,  8,  1,
   51,   1, 11,  2,
   76,   4, 36,  9,
  102,   6, 66, 18,
  127,  27, 95, 23,
  153,  82,127, 31,
  178, 197,171, 40,
  204, 133,100, 19,
  229,  97, 48,  6,
  255, 163, 55,  7};


DEFINE_GRADIENT_PALETTE( garish14_gp ) {
    0,   7,139,  6,
    2,   7,139,  6,
    2,  39,162,  9,
    4,  39,162,  9,
    4, 106,189, 12,
    6, 106,189, 12,
    6, 159,203,  5,
    8, 159,203,  5,
    8, 227,217,  1,
   12, 227,217,  1,
   12, 232,182,  1,
   17, 232,182,  1,
   17, 239,151,  1,
   28, 239,151,  1,
   28, 239,101,  1,
   42, 239,101,  1,
   42, 239, 61,  0,
   63, 239, 61,  0,
   63, 140, 29,  7,
   84, 140, 29,  7,
   84,  72,  9, 42,
  127,  72,  9, 42,
  127, 112, 39, 92,
  170, 112, 39, 92,
  170, 163, 93,164,
  212, 163, 93,164,
  212, 194,149,197,
  255, 194,149,197};

DEFINE_GRADIENT_PALETTE( precip2_17lev_gp ) {
    0, 255,255,255,
   14, 255,255,255,
   14, 206,235,255,
   28, 206,235,255,
   28, 161,195,255,
   42, 161,195,255,
   42, 104,151,255,
   56, 104,151,255,
   56,  55,115,255,
   70,  55,115,255,
   70,  41, 79,255,
   84,  41, 79,255,
   84,  31, 60,237,
   99,  31, 60,237,
   99,  21, 41,237,
  113,  21, 41,237,
  113,   0, 88,  1,
  127,   0, 88,  1,
  127,   5,130,  7,
  141,   5,130,  7,
  141, 101,164, 31,
  155, 101,164, 31,
  155, 110,241, 31,
  170, 110,241, 31,
  170, 255,241,  1,
  184, 255,241,  1,
  184, 255, 95,  1,
  198, 255, 95,  1,
  198, 192,  0,  0,
  212, 192,  0,  0,
  212, 117,  0,  0,
  226, 117,  0,  0,
  226,  43,  0,  0,
  240,  43,  0,  0,
  240,   0,  0,  0,
  255,   0,  0,  0};


DEFINE_GRADIENT_PALETTE( humidity_gp ) {
    0,   3,  0, 43,
   10,   3,  0, 43,
   10,   0,  0, 46,
   20,   0,  0, 46,
   20,   0,  7,105,
   30,   0,  7,105,
   30,   0,  0,255,
   40,   0,  0,255,
   40,   0, 53,255,
   51,   0, 53,255,
   51,   0,131,255,
   61,   0,131,255,
   61,   1,175, 55,
   71,   1,175, 55,
   71,   5,108, 34,
   81,   5,108, 34,
   81,   2,103,  2,
   91,   2,103,  2,
   91,   2,149,  2,
  102,   2,149,  2,
  102,   0,255,  4,
  112,   0,255,  4,
  112,  13,255,  0,
  122,  13,255,  0,
  122,  74,255,  0,
  132,  74,255,  0,
  132, 255,255,  0,
  142, 255,255,  0,
  142, 255,156,  0,
  153, 255,156,  0,
  153, 255, 81,  0,
  163, 255, 81,  0,
  163, 255, 33,  0,
  173, 255, 33,  0,
  173, 255, 22,  0,
  183, 255, 22,  0,
  183, 255,  0,  0,
  193, 255,  0,  0,
  193, 255,  0, 44,
  204, 255,  0, 44,
  204, 255,  3, 61,
  214, 255,  3, 61,
  214, 255, 28,102,
  224, 255, 28,102,
  224, 255, 61,128,
  234, 255, 61,128,
  234, 255,109,166,
  244, 255,109,166,
  244, 255,175,207,
  255, 255,175,207};


DEFINE_GRADIENT_PALETTE( wiki_ice_greenland_gp ) {
    0, 234,250,252,
   79, 234,250,252,
   79, 222,239,237,
  118, 222,239,237,
  118, 192,223,223,
  158, 192,223,223,
  158, 167,205,203,
  198, 167,205,203,
  198, 148,193,192,
  238, 148,193,192,
  238, 126,182,182,
  255, 126,182,182};


DEFINE_GRADIENT_PALETTE( GMT_gebco_gp ) {
    0,   0,223,255,
   36,   0,223,255,
   36,   1,255,255,
   72,   1,255,255,
   72,  17,255,255,
  109,  17,255,255,
  109,  53,255,197,
  145,  53,255,197,
  145,  82,255,166,
  182,  82,255,166,
  182, 126,255,166,
  218, 126,255,166,
  218, 153,255,166,
  236, 153,255,166,
  236, 194,255,219,
  247, 194,255,219,
  247, 206,255,255,
  255, 206,255,255};

DEFINE_GRADIENT_PALETTE( GMT_ocean_gp ) {
    0,   0,  0,  0,
   31,   0,  1,  1,
   63,   0,  1,  4,
   95,   0, 19, 42,
  127,   0, 79,138,
  159,  15,144,112,
  191,  91,233, 89,
  223, 155,244,158,
  255, 242,255,255};


DEFINE_GRADIENT_PALETTE( radioactive_slime_gp ) {
    0, 255,233,102,
   25, 255,233,102,
   25, 255,195, 87,
   51, 255,195, 87,
   51, 179,191, 82,
   76, 179,191, 82,
   76, 104,195, 78,
  102, 104,195, 78,
  102,  50,186, 65,
  127,  50,186, 65,
  127,  24,141, 34,
  153,  24,141, 34,
  153,   9, 91, 12,
  178,   9, 91, 12,
  178,  14, 56, 12,
  204,  14, 56, 12,
  204,  11, 37,  9,
  229,  11, 37,  9,
  229,   3, 27,  3,
  255,   3, 27,  3};

DEFINE_GRADIENT_PALETTE( bhw1_05_gp ) {
    0,   1,221, 53,
  255,  73,  3,178};


DEFINE_GRADIENT_PALETTE( bhw2_sherbet2_gp ) {
    0, 217,  1,  1,
   35, 249, 43, 19,
   71, 247,125,172,
  109, 206,  2, 32,
  163, 210, 23,  9,
  211, 255,255,255,
  232, 252,199, 88,
  255, 206,115, 52};


DEFINE_GRADIENT_PALETTE( bhw2_grrrrr_gp ) {
    0, 184, 15,155,
   35,  78, 46,168,
   84,  65,169,230,
  130,   9,127,186,
  163,  77,182,109,
  191, 242,246, 55,
  216, 142,128,103,
  255,  72, 50,168};


DEFINE_GRADIENT_PALETTE( bhw3_63_gp ) {
    0, 255,255,255,
   94, 177,122,182,
  112, 255,255,255,
  127, 128,237,130,
  145, 255,255,255,
  163, 137,166,245,
  181, 255,255,255,
  198, 255,255, 45,
  214, 255,255,255,
  232, 249,168,219,
  255, 255,255,255};

DEFINE_GRADIENT_PALETTE( bhw1_14_gp ) {
    0,   0,  0,  0,
   12,   1,  1,  3,
   53,   8,  1, 22,
   80,   4,  6, 89,
  119,   2, 25,216,
  145,   7, 10, 99,
  186,  15,  2, 31,
  233,   2,  1,  5,
  255,   0,  0,  0};

// Palette initialization
CRGBPalette16 myPal1 = heatmap_gp;
CRGBPalette16 myPal2 = rainbow_gp;
CRGBPalette16 myPal3 = purplefly_gp;
CRGBPalette16 myPal4 = bhw2_39_gp;
CRGBPalette16 myPal5 = bhw1_01_gp;
CRGBPalette16 myPal6 = teabearrose_gp;
CRGBPalette16 myPal7 = Need_I_Say_More_gp;
CRGBPalette16 myPal8 = aurora_gp;
CRGBPalette16 myPal9 = nrwc_gp;
CRGBPalette16 myPal10 = garish14_gp;
CRGBPalette16 myPal11 = precip2_17lev_gp;
CRGBPalette16 myPal12 = humidity_gp;
//CRGBPalette16 myPal13 = bhw2_39_gp;
CRGBPalette16 myPal14 = bhw3_63_gp;
CRGBPalette16 myPal15 = GMT_gebco_gp;
CRGBPalette16 myPal16 = GMT_ocean_gp;
CRGBPalette16 myPal17 = radioactive_slime_gp;
CRGBPalette16 myPal18 = bhw1_05_gp;
CRGBPalette16 myPal19 = bhw2_sherbet2_gp;
CRGBPalette16 myPal20 = bhw2_grrrrr_gp;
CRGBPalette16 myPal21 = wiki_ice_greenland_gp;


CRGBPalette16 galaxyPal2 = bhw1_14_gp;
// Palette array
CRGBPalette16 palletes[] = {myPal20,myPal7,myPal3,myPal14,myPal6,myPal15,myPal1,myPal11,myPal18,myPal2,myPal9, myPal19, myPal4, myPal17, myPal10, myPal5, myPal16, myPal8, myPal12, myPal21};


void setup() { 
    // LED setup
    // CHANGE
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, 22>(leds, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 23>(leds2, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 15>(leds3, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 25>(leds4, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 26>(leds5, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 27>(leds6, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 16>(leds7, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 17>(leds8, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 18>(leds9, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 19>(leds10, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 21>(leds11, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, 14>(leds12, NUM_LEDS);

    //FastLED.setBrightness(50);
    // Sensor setup
    //Serial.begin(9600);
    //Serial.println("HX711 calibration sketch");
    //Serial.println("Remove all weight from scale");
    //Serial.println("After readings begin, place known weight on scale");
    //Serial.println("Press + or a to increase calibration factor");
    //Serial.println("Press - or z to decrease calibration factor");

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale();
    scale.tare();                         //Reset the scale to 0
    scale.set_scale(calibration_factor);  //Adjust to this calibration factor
    Serial.println("Starting: Init scale reset");

    long zero_factor = scale.read_average(); //Get a baseline reading
    //Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    //Serial.println(zero_factor);
}


void loop() {
  //scale.set_scale(calibration_factor); //Adjust to this calibration factor
  scale.set_scale(calibration_factor);
  float force = scale.get_units();
  float force2 = scale.get_units();
  Serial.println("OKAY");
  Serial.println(force);
  //Serial.print(scale.read_average());
  //Serial.print("\n");
  //Serial.println(scale.get_units());
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }

  if ((force < -3) & (force > -150)){
    patternMode = rand()%20;
    for(uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds2[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds3[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds4[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds5[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds6[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds7[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds8[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds9[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds10[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds11[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds12[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      }
    FastLED.show();
    // Cue different passive patterns
      while ((force < -10) & (force > -300)){
        Serial.println("standing");
        Serial.println(force);
        Serial.println("\n");
        EVERY_N_MILLISECONDS(1500){
          patternMode = rand()%20;
          for(uint8_t i = 0; i < NUM_LEDS; i++) {
            leds[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds2[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds3[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds4[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds5[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds6[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds7[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds8[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds9[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds10[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds11[i] = ColorFromPalette(palletes[patternMode], i*2.5);
            leds12[i] = ColorFromPalette(palletes[patternMode], i*2.5);
          }
          FastLED.show();
          
        }
        force =  scale.get_units(3);
        force2 =  scale.get_units();
    }
  }

  if ((force-force2 >  5) || (force2-force >  5)){
    switch (patternCounter){
      case 0:
        //randomFlickering();
        //rainbowDot();
        sineWave();
        break;
      case 1:
        galaxy();
        break;
      case 2:
        upwardMovement();
        //cascade();
        break;
      case 3:
        towersPulsing();
        break;
      case 4:
        randomFlickering();
        break;
      case 5:
        dotMoving();
        break;
      case 6:
        downwardMovement();
        break;
      //case 7:
      //  towersPulsingPalette();
      //  break;
      //case 2: 
      //  lightsFlickering();
      //  break;
    }
    nextPattern();
    force = scale.get_units();
    force2 = 0;
  }

  EVERY_N_MILLISECONDS(18000){
    spacing = rand() % 800;
  }

// Raindrops
uint8_t sawPos = map(255 - beat8(speed_array[speed_index], spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t sawPos2 = map(255 - beat8(speed_array[speed_index],  500 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t sawPos3 = map(255 - beat8(speed_array[speed_index],  1000 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t sawPos4 = map(255 - beat8(speed_array[speed_index],  1500 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t sawPos5 = map(255 - beat8(speed_array[speed_index],  2000 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t sawPos6 = map(255 - beat8(speed_array[speed_index],  2500 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t sawPos7 = map(255 - beat8(speed_array[speed_index],  3000 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t sawPos8 = map(255 - beat8(speed_array[speed_index],  3500 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t sawPos9 = map(255 - beat8(speed_array[speed_index],  4000 + spacing), 0, 255, 0, NUM_LEDS - 1);

uint8_t wsawPos = map(255 - beat8(speed_array[speed_index], spacing*0.2), 0, 255, 0, NUM_LEDS - 1);
uint8_t wsawPos2 = map(255 - beat8(speed_array[speed_index],  500 + spacing * 0.35), 0, 255, 0, NUM_LEDS - 1);
uint8_t wsawPos3 = map(255 - beat8(speed_array[speed_index],  1000 + spacing * 0.6), 0, 255, 0, NUM_LEDS - 1);
uint8_t wsawPos4 = map(255 - beat8(speed_array[speed_index],  1500 + spacing * 0.2), 0, 255, 0, NUM_LEDS - 1);
uint8_t wsawPos5 = map(255 - beat8(speed_array[speed_index],  2000 + spacing * 0.8), 0, 255, 0, NUM_LEDS - 1);
uint8_t wsawPos6 = map(255 - beat8(speed_array[speed_index],  2500 + spacing * 0.67), 0, 255, 0, NUM_LEDS - 1);
uint8_t wsawPos7 = map(255 - beat8(speed_array[speed_index],  3000 + spacing * 0.9), 0, 255, 0, NUM_LEDS - 1);
uint8_t wsawPos8 = map(255 - beat8(speed_array[speed_index],  3500 + spacing * 0.12), 0, 255, 0, NUM_LEDS - 1);
uint8_t wsawPos9 = map(255 - beat8(speed_array[speed_index],  4000 + spacing * 0.45), 0, 255, 0, NUM_LEDS - 1);

uint8_t esawPos = map(255 - beat8(speed_array[speed_index], spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t esawPos2 = map(255 - beat8(speed_array[speed_index],  500 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t esawPos3 = map(255 - beat8(speed_array[speed_index],  1000 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t esawPos4 = map(255 - beat8(speed_array[speed_index],  1500 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t esawPos5 = map(255 - beat8(speed_array[speed_index],  2000 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t esawPos6 = map(255 - beat8(speed_array[speed_index],  2500 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t esawPos7 = map(255 - beat8(speed_array[speed_index],  3000 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t esawPos8 = map(255 - beat8(speed_array[speed_index],  3500 + spacing), 0, 255, 0, NUM_LEDS - 1);
uint8_t esawPos9 = map(255 - beat8(speed_array[speed_index],  4000 + spacing), 0, 255, 0, NUM_LEDS - 1);

uint8_t qsawPos = map(255 - beat8(speed_array[speed_index], spacing*0.2), 0, 255, 0, NUM_LEDS - 1);
uint8_t qsawPos2 = map(255 - beat8(speed_array[speed_index],  500 + spacing * 0.35), 0, 255, 0, NUM_LEDS - 1);
uint8_t qsawPos3 = map(255 - beat8(speed_array[speed_index],  1000 + spacing * 0.6), 0, 255, 0, NUM_LEDS - 1);
uint8_t qsawPos4 = map(255 - beat8(speed_array[speed_index],  1500 + spacing * 0.2), 0, 255, 0, NUM_LEDS - 1);
uint8_t qsawPos5 = map(255 - beat8(speed_array[speed_index],  2000 + spacing * 0.8), 0, 255, 0, NUM_LEDS - 1);
uint8_t qsawPos6 = map(255 - beat8(speed_array[speed_index],  2500 + spacing * 0.67), 0, 255, 0, NUM_LEDS - 1);
uint8_t qsawPos7 = map(255 - beat8(speed_array[speed_index],  3000 + spacing * 0.9), 0, 255, 0, NUM_LEDS - 1);
uint8_t qsawPos8 = map(255 - beat8(speed_array[speed_index],  3500 + spacing * 0.12), 0, 255, 0, NUM_LEDS - 1);
uint8_t qsawPos9 = map(255 - beat8(speed_array[speed_index],  4000 + spacing * 0.45), 0, 255, 0, NUM_LEDS - 1);
  // background colour
  fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds2, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds3, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds4, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds5, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds6, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds7, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds8, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds9, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds10, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds11, NUM_LEDS, CRGB(0,0,0));
  fill_solid(leds12, NUM_LEDS, CRGB(0,0,0));

  // Raindrop colour
  leds[sawPos]   = CRGB(247,247,247);
  leds[sawPos2]   = CRGB(247,247,247);
  leds[sawPos3]   = CRGB(247,247,247);
  leds[sawPos4]   = CRGB(247,247,247);
  leds[sawPos5]   = CRGB(247,247,247);
  leds[sawPos6]   = CRGB(247,247,247);
  leds[sawPos7]   = CRGB(247,247,247);
  leds[sawPos8]   = CRGB(247,247,247);
  leds[sawPos9]   = CRGB(247,247,247);

  leds2[wsawPos]   = CRGB(247,247,247);
  leds2[wsawPos2]   = CRGB(247,247,247);
  leds2[wsawPos3]   = CRGB(247,247,247);
  leds2[wsawPos4]   = CRGB(247,247,247);
  leds2[wsawPos5]   = CRGB(247,247,247);
  leds2[wsawPos6]   = CRGB(247,247,247);
  leds2[wsawPos7]   = CRGB(247,247,247);
  leds2[wsawPos8]   = CRGB(247,247,247);
  leds2[wsawPos9]   = CRGB(247,247,247);

  leds3[esawPos]   = CRGB(247,247,247);
  leds3[esawPos2]   = CRGB(247,247,247);
  leds3[esawPos3]   = CRGB(247,247,247);
  leds3[esawPos4]   = CRGB(247,247,247);
  leds3[esawPos5]   = CRGB(247,247,247);
  leds3[esawPos6]   = CRGB(247,247,247);
  leds3[esawPos7]   = CRGB(247,247,247);
  leds3[esawPos8]   = CRGB(247,247,247);
  leds3[esawPos9]   = CRGB(247,247,247);

  leds4[qsawPos]   = CRGB(247,247,247);
  leds4[qsawPos2]   = CRGB(247,247,247);
  leds4[qsawPos3]   = CRGB(247,247,247);
  leds4[qsawPos4]   = CRGB(247,247,247);
  leds4[qsawPos5]   = CRGB(247,247,247);
  leds4[qsawPos6]   = CRGB(247,247,247);
  leds4[qsawPos7]   = CRGB(247,247,247);
  leds4[qsawPos8]   = CRGB(247,247,247);
  leds4[qsawPos9]   = CRGB(247,247,247);

  leds5[sawPos]   = CRGB(247,247,247);
  leds5[sawPos2]   = CRGB(247,247,247);
  leds5[sawPos3]   = CRGB(247,247,247);
  leds5[sawPos4]   = CRGB(247,247,247);
  leds5[sawPos5]   = CRGB(247,247,247);
  leds5[sawPos6]   = CRGB(247,247,247);
  leds5[sawPos7]   = CRGB(247,247,247);
  leds5[sawPos8]   = CRGB(247,247,247);
  leds5[sawPos9]   = CRGB(247,247,247);

  leds6[wsawPos]   = CRGB(247,247,247);
  leds6[wsawPos2]   = CRGB(247,247,247);
  leds6[wsawPos3]   = CRGB(247,247,247);
  leds6[wsawPos4]   = CRGB(247,247,247);
  leds6[wsawPos5]   = CRGB(247,247,247);
  leds6[wsawPos6]   = CRGB(247,247,247);
  leds6[wsawPos7]   = CRGB(247,247,247);
  leds6[wsawPos8]   = CRGB(247,247,247);
  leds6[wsawPos9]   = CRGB(247,247,247);

  leds7[esawPos]   = CRGB(247,247,247);
  leds7[esawPos2]   = CRGB(247,247,247);
  leds7[esawPos3]   = CRGB(247,247,247);
  leds7[esawPos4]   = CRGB(247,247,247);
  leds7[esawPos5]   = CRGB(247,247,247);
  leds7[esawPos6]   = CRGB(247,247,247);
  leds7[esawPos7]   = CRGB(247,247,247);
  leds7[esawPos8]   = CRGB(247,247,247);
  leds7[esawPos9]   = CRGB(247,247,247);

  leds8[qsawPos]   = CRGB(247,247,247);
  leds8[qsawPos2]   = CRGB(247,247,247);
  leds8[qsawPos3]   = CRGB(247,247,247);
  leds8[qsawPos4]   = CRGB(247,247,247);
  leds8[qsawPos5]   = CRGB(247,247,247);
  leds8[qsawPos6]   = CRGB(247,247,247);
  leds8[qsawPos7]   = CRGB(247,247,247);
  leds8[qsawPos8]   = CRGB(247,247,247);
  leds8[qsawPos9]   = CRGB(247,247,247);

  leds9[sawPos]   = CRGB(247,247,247);
  leds9[sawPos2]   = CRGB(247,247,247);
  leds9[sawPos3]   = CRGB(247,247,247);
  leds9[sawPos4]   = CRGB(247,247,247);
  leds9[sawPos5]   = CRGB(247,247,247);
  leds9[sawPos6]   = CRGB(247,247,247);
  leds9[sawPos7]   = CRGB(247,247,247);
  leds9[sawPos8]   = CRGB(247,247,247);
  leds9[sawPos9]   = CRGB(247,247,247);

  leds10[wsawPos]   = CRGB(247,247,247);
  leds10[wsawPos2]   = CRGB(247,247,247);
  leds10[wsawPos3]   = CRGB(247,247,247);
  leds10[wsawPos4]   = CRGB(247,247,247);
  leds10[wsawPos5]   = CRGB(247,247,247);
  leds10[wsawPos6]   = CRGB(247,247,247);
  leds10[wsawPos7]   = CRGB(247,247,247);
  leds10[wsawPos8]   = CRGB(247,247,247);
  leds10[wsawPos9]   = CRGB(247,247,247);

  leds11[esawPos]   = CRGB(247,247,247);
  leds11[esawPos2]   = CRGB(247,247,247);
  leds11[esawPos3]   = CRGB(247,247,247);
  leds11[esawPos4]   = CRGB(247,247,247);
  leds11[esawPos5]   = CRGB(247,247,247);
  leds11[esawPos6]   = CRGB(247,247,247);
  leds11[esawPos7]   = CRGB(247,247,247);
  leds11[esawPos8]   = CRGB(247,247,247);
  leds11[esawPos9]   = CRGB(247,247,247);

  leds12[qsawPos]   = CRGB(247,247,247);
  leds12[qsawPos2]   = CRGB(247,247,247);
  leds12[qsawPos3]   = CRGB(247,247,247);
  leds12[qsawPos4]   = CRGB(247,247,247);
  leds12[qsawPos5]   = CRGB(247,247,247);
  leds12[qsawPos6]   = CRGB(247,247,247);
  leds12[qsawPos7]   = CRGB(247,247,247);
  leds12[qsawPos8]   = CRGB(247,247,247);
  leds12[qsawPos9]   = CRGB(247,247,247);

  EVERY_N_MILLISECONDS(5){
    for(int i = 0; i < 4; i++) {
      blur1d(leds, NUM_LEDS, 40);
      blur1d(leds2, NUM_LEDS, 40);
      blur1d(leds3, NUM_LEDS, 40);
      blur1d(leds4, NUM_LEDS, 40);
      blur1d(leds5, NUM_LEDS, 40);
      blur1d(leds6, NUM_LEDS, 40);
      blur1d(leds7, NUM_LEDS, 40);
      blur1d(leds8, NUM_LEDS, 40);
      blur1d(leds9, NUM_LEDS, 40);
      blur1d(leds10, NUM_LEDS, 40);
      blur1d(leds11, NUM_LEDS, 40);
      blur1d(leds12, NUM_LEDS, 40);
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


/// Patterns
void nextPattern() {
  patternCounter = (patternCounter + 1) % 7;          // Change the number after the % to the number of patterns you have
}
void upwardMovement(){
    patternMode = rand()%20;
    for(uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds2[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds3[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds4[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds5[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds6[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds7[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds8[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds9[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds10[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds11[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds12[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      FastLED.show();
      delay(20);
    }
}

void downwardMovement(){
    patternMode = rand()%20;
    for(uint8_t i = NUM_LEDS-1; i > 0; i--) {
      leds[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds2[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds3[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds4[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds5[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds6[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds7[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds8[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds9[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds10[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds11[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      leds12[i] = ColorFromPalette(palletes[patternMode], i*2.5);
      FastLED.show();
      delay(20);
    }
}

void paletteFill(){
    uint16_t beatA = beatsin16(30, 0, 255);
    uint16_t beatB = beatsin16(20, 0, 255);
    patternMode = rand()%20;
    for(uint8_t i = 0; i < NUM_LEDS; i++){
      fill_palette(leds, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds2, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds3, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds4, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds5, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds6, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds7, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds8, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds9, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds10, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds11, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      fill_palette(leds12, NUM_LEDS, (beatA + beatB) / 2, 10, palletes[patternMode], 255, LINEARBLEND);
      FastLED.show();
      delay(20);
    }
}


void shiftingColoursUp(){
  patternMode = rand()%20;
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t brightness = inoise8(i * brightnessScale, millis() / 5);
    uint8_t index = inoise8(i * indexScale, millis() /10);
    leds[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds2[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds3[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds4[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds5[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds6[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds7[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds8[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds9[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds10[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds11[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds12[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    delay(20);
    FastLED.show();
  }
}

void shiftingColoursDown(){
  patternMode = rand()%20;
  for (int i = NUM_LEDS-1; i > 0; i--) {
    uint8_t brightness = inoise8(i * brightnessScale, millis() / 5);
    uint8_t index = inoise8(i * indexScale, millis() /10);
    leds[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds2[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds3[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds4[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds5[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds6[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds7[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds8[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds9[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds10[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds11[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    leds12[i] = ColorFromPalette(palletes[patternMode], index, brightness);
    //leds[i] = CHSV(0, 255, brightness);
    delay(20);
    FastLED.show();
  }
}

void gorgeousFill(){
  uint8_t octaves = 1;
  uint16_t x = 0;
  int scale = 100;
  uint8_t hue_octaves = 1;
  uint16_t hue_x = 1;
  int hue_scale = 50;
  uint16_t ntime = millis() / 3;
  uint8_t hue_shift = 5;
  for (int i = 0; i < NUM_LEDS; i++){
    fill_noise16 (leds, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds2, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds3, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds4, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds5, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds6, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds7, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds8, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds9, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds10, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds11, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    fill_noise16 (leds12, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
    FastLED.show();
    delay(20);
  }
}


// Seje patterns!! 
void dotMoving(){
  for (int i = 0; i < NUM_LEDS*1.5; i++){
    drawBackground();
    drawMovingPixel();

    EVERY_N_MILLISECONDS(20) {
      fadeToBlackBy(leds, NUM_LEDS, 10); 
      nblend(leds, background, NUM_LEDS, 30);
      fadeToBlackBy(leds2, NUM_LEDS, 10); 
      nblend(leds2, background, NUM_LEDS, 30);
      fadeToBlackBy(leds3, NUM_LEDS, 10); 
      nblend(leds3, background, NUM_LEDS, 30);
      fadeToBlackBy(leds4, NUM_LEDS, 10); 
      nblend(leds4, background, NUM_LEDS, 30);
      fadeToBlackBy(leds5, NUM_LEDS, 10); 
      nblend(leds5, background, NUM_LEDS, 30);
      fadeToBlackBy(leds6, NUM_LEDS, 10); 
      nblend(leds6, background, NUM_LEDS, 30);
      fadeToBlackBy(leds7, NUM_LEDS, 10); 
      nblend(leds7, background, NUM_LEDS, 30);
      fadeToBlackBy(leds8, NUM_LEDS, 10); 
      nblend(leds8, background, NUM_LEDS, 30);
      fadeToBlackBy(leds9, NUM_LEDS, 10); 
      nblend(leds9, background, NUM_LEDS, 30);
      fadeToBlackBy(leds10, NUM_LEDS, 10); 
      nblend(leds10, background, NUM_LEDS, 30);
      fadeToBlackBy(leds11, NUM_LEDS, 10); 
      nblend(leds11, background, NUM_LEDS, 30);
      fadeToBlackBy(leds12, NUM_LEDS, 10); 
      nblend(leds12, background, NUM_LEDS, 30);
    } 
    delay(20);
    FastLED.show();
  }
}

void drawBackground() {
  // A simple plasma effect
  fill_noise16 (background, NUM_LEDS, 1, millis(), 30, 1, 0, 50, millis() / 3, 10);
}

void drawMovingPixel() {
  // A pixel that moves back and forth using noise
  uint16_t pos = inoise16(millis() * 100);
  pos = constrain(pos, 13000, 51000);
  pos = map(pos, 13000, 51000, 0, NUM_LEDS - 1);
  leds[pos] = CRGB::Red;
  leds2[pos] = CRGB::Red;
  leds3[pos] = CRGB::Red;
  leds4[pos] = CRGB::Red;
  leds5[pos] = CRGB::Red;
  leds6[pos] = CRGB::Red;
  leds7[pos] = CRGB::Red;
  leds8[pos] = CRGB::Red;
  leds9[pos] = CRGB::Red;
  leds10[pos] = CRGB::Red;
  leds11[pos] = CRGB::Red;
  leds12[pos] = CRGB::Red;
}
// We can make a different sinewave for every LED strip.
void sineWave(){
  for (int i = 0; i < 200; i++){
    //fill_solid(leds, NUM_LEDS, CRGB(0,40,40));
    //fill_solid(leds2, NUM_LEDS, CRGB(0,40,40));
    uint16_t sinBeat = beatsin16(10, 0, NUM_LEDS - 1, 0, 0);

    leds[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds2[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds2[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds2[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds3[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds3[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds3[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds4[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds4[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds4[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds5[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds5[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds5[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds6[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds6[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds6[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds7[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds7[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds7[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds8[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds8[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds8[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds9[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds9[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds9[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds10[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds10[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds10[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds11[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds11[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds11[constrain(sinBeat+2,0,99)] = CRGB::Purple;
    leds12[constrain(sinBeat,0,99)] = CRGB::Purple;
    leds12[constrain(sinBeat+1,0,99)] = CRGB::Purple;
    leds12[constrain(sinBeat+2,0,99)] = CRGB::Purple;
  
    fadeToBlackBy(leds, NUM_LEDS, 10);
    fadeToBlackBy(leds2, NUM_LEDS, 10);
    fadeToBlackBy(leds3, NUM_LEDS, 10);
    fadeToBlackBy(leds4, NUM_LEDS, 10);
    fadeToBlackBy(leds5, NUM_LEDS, 10);
    fadeToBlackBy(leds6, NUM_LEDS, 10);
    fadeToBlackBy(leds7, NUM_LEDS, 10);
    fadeToBlackBy(leds8, NUM_LEDS, 10);
    fadeToBlackBy(leds9, NUM_LEDS, 10);
    fadeToBlackBy(leds10, NUM_LEDS, 10);
    fadeToBlackBy(leds11, NUM_LEDS, 10);
    fadeToBlackBy(leds12, NUM_LEDS, 10);
    delay(20);
    FastLED.show();
  }
}

void randomFlickering(){
  uint8_t scale = beatsin8(10,10,30);
    for (int i = 0; i < 150; i++){
        fadeToBlackBy(leds, NUM_LEDS, 10);
        fadeToBlackBy(leds2, NUM_LEDS, 10);
        fadeToBlackBy(leds3, NUM_LEDS, 10);
        fadeToBlackBy(leds4, NUM_LEDS, 10);
        fadeToBlackBy(leds5, NUM_LEDS, 10);
        fadeToBlackBy(leds6, NUM_LEDS, 10);
        fadeToBlackBy(leds7, NUM_LEDS, 10);
        fadeToBlackBy(leds8, NUM_LEDS, 10);
        fadeToBlackBy(leds9, NUM_LEDS, 10);
        fadeToBlackBy(leds10, NUM_LEDS, 10);
        fadeToBlackBy(leds11, NUM_LEDS, 10);
        fadeToBlackBy(leds12, NUM_LEDS, 10);
        int index_noise = map(inoise8(i*scale),0,255,0,99);
        leds[index_noise] = CRGB::Red;
        leds2[index_noise] = CRGB::Red;
        leds3[index_noise] = CRGB::Red;
        leds4[index_noise] = CRGB::Red;
        leds5[index_noise] = CRGB::Red;
        leds6[index_noise] = CRGB::Red;
        leds7[index_noise] = CRGB::Red;
        leds8[index_noise] = CRGB::Red;
        leds9[index_noise] = CRGB::Red;
        leds10[index_noise] = CRGB::Red;
        leds11[index_noise] = CRGB::Red;
        leds12[index_noise] = CRGB::Red;
        delay(20);
        FastLED.show();
     }
}

void towersPulsing(){
  fill_solid(leds, 99,CRGB(0,0,0));
  fill_solid(leds2, 99,CRGB(0,0,0));
  fill_solid(leds3, 99,CRGB(0,0,0));
  fill_solid(leds4, 99,CRGB(0,0,0));
  fill_solid(leds5, 99,CRGB(0,0,0));
  fill_solid(leds6, 99,CRGB(0,0,0));
  fill_solid(leds7, 99,CRGB(0,0,0));
  fill_solid(leds8, 99,CRGB(0,0,0));
  fill_solid(leds9, 99,CRGB(0,0,0));
  fill_solid(leds10, 99,CRGB(0,0,0));
  fill_solid(leds11, 99,CRGB(0,0,0));
  fill_solid(leds12, 99,CRGB(0,0,0));
  int num_leds_1 = 70;
  int num_leds_2 = 45;
  int num_leds_3 = 20;
  int num_leds_4 = 60;
  int num_leds_5 = 81;
  int num_leds_6 = 99;
  int num_leds_7 = 37;
  int num_leds_8 = 10;
  int num_leds_9 = 44;
  int num_leds_10 = 57;
  int num_leds_11 = 70;
  int num_leds_12 = 17;
  for (int i = 0; i < 50; i++){
    int random_num_1 =  random(-5,5);
    int random_num_2 =  random(-5,5);
    int random_num_3 =  random(-5,5);
    int random_num_4 =  random(-5,5);
    int random_num_5 =  random(-5,5);
    int random_num_6 =  random(-5,5);
    int random_num_7 =  random(-5,5);
    int random_num_8 =  random(-5,5);
    int random_num_9 =  random(-5,5);
    int random_num_10 =  random(-5,5);
    int random_num_11 =  random(-5,5);
    int random_num_12 =  random(-5,5);
    fill_solid(leds, num_leds_1,CRGB(120,120,0));
    fill_solid(leds2, num_leds_2,CRGB(120,120,0));
    fill_solid(leds3, num_leds_3,CRGB(120,120,0));
    fill_solid(leds4, num_leds_4,CRGB(120,120,0));
    fill_solid(leds5, num_leds_5,CRGB(120,120,0));
    fill_solid(leds6, num_leds_6,CRGB(120,120,0));
    fill_solid(leds7, num_leds_7,CRGB(120,120,0));
    fill_solid(leds8, num_leds_8,CRGB(120,120,0));
    fill_solid(leds9, num_leds_9,CRGB(120,120,0));
    fill_solid(leds10, num_leds_10,CRGB(120,120,0));
    fill_solid(leds11, num_leds_11,CRGB(120,120,0));
    fill_solid(leds12, num_leds_12,CRGB(120,120,0));
    num_leds_1 = num_leds_1 + random_num_1;
    num_leds_2 = num_leds_2 + random_num_2;
    num_leds_3 = num_leds_3 + random_num_3;
    num_leds_4 = num_leds_4 + random_num_4;
    num_leds_5 = num_leds_5 + random_num_5;
    num_leds_6 = num_leds_6 + random_num_6;
    num_leds_7 = num_leds_7 + random_num_7;
    num_leds_8 = num_leds_8 + random_num_8;
    num_leds_9 = num_leds_9 + random_num_9;
    num_leds_10 = num_leds_10 + random_num_10;
    num_leds_11 = num_leds_11 + random_num_11;
    num_leds_12 = num_leds_12 + random_num_12;
    delay(10);
    FastLED.show();
    for (int i = 0; i < 10; i++){
      fadeToBlackBy(leds, NUM_LEDS, 10);
      fadeToBlackBy(leds2, NUM_LEDS, 10);
      fadeToBlackBy(leds3, NUM_LEDS, 10);
      fadeToBlackBy(leds4, NUM_LEDS, 10);
      fadeToBlackBy(leds5, NUM_LEDS, 10);
      fadeToBlackBy(leds6, NUM_LEDS, 10);
      fadeToBlackBy(leds7, NUM_LEDS, 10);
      fadeToBlackBy(leds8, NUM_LEDS, 10);
      fadeToBlackBy(leds9, NUM_LEDS, 10);
      fadeToBlackBy(leds10, NUM_LEDS, 10);
      fadeToBlackBy(leds11, NUM_LEDS, 10);
      fadeToBlackBy(leds12, NUM_LEDS, 10);
      delay(10);
      FastLED.show();
    }
  }
}


void towersPulsingPalette(){
  fill_solid(leds, 99,CRGB(0,0,0));
  fill_solid(leds2, 99,CRGB(0,0,0));
  fill_solid(leds3, 99,CRGB(0,0,0));
  fill_solid(leds4, 99,CRGB(0,0,0));
  fill_solid(leds5, 99,CRGB(0,0,0));
  fill_solid(leds6, 99,CRGB(0,0,0));
  fill_solid(leds7, 99,CRGB(0,0,0));
  fill_solid(leds8, 99,CRGB(0,0,0));
  fill_solid(leds9, 99,CRGB(0,0,0));
  fill_solid(leds10, 99,CRGB(0,0,0));
  fill_solid(leds11, 99,CRGB(0,0,0));
  fill_solid(leds12, 99,CRGB(0,0,0));
  int num_leds_1 = 70;
  int num_leds_2 = 45;
  int num_leds_3 = 20;
  int num_leds_4 = 60;
  int num_leds_5 = 81;
  int num_leds_6 = 99;
  int num_leds_7 = 37;
  int num_leds_8 = 10;
  int num_leds_9 = 44;
  int num_leds_10 = 57;
  int num_leds_11 = 70;
  int num_leds_12 = 17;
  for (int i = 0; i < 30; i++){
    int random_num_1 =  random(-5,5);
    int random_num_2 =  random(-5,5);
    int random_num_3 =  random(-5,5);
    int random_num_4 =  random(-5,5);
    int random_num_5 =  random(-5,5);
    int random_num_6 =  random(-5,5);
    int random_num_7 =  random(-5,5);
    int random_num_8 =  random(-5,5);
    int random_num_9 =  random(-5,5);
    int random_num_10 =  random(-5,5);
    int random_num_11 =  random(-5,5);
    int random_num_12 =  random(-5,5);
    fill_gradient(leds, num_leds_1,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds2, num_leds_2,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds3, num_leds_3,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds4, num_leds_4,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds5, num_leds_5,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds6, num_leds_6,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds7, num_leds_7,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds8, num_leds_8,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds9, num_leds_9,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds10, num_leds_10,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds11, num_leds_11,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    fill_gradient(leds12, num_leds_12,CHSV(50, 255, 255) ,CHSV(100, 255, 255) ,CHSV(150, 255, 255) ,CHSV(200, 255, 255)); 
    num_leds_1 = num_leds_1 + random_num_1;
    num_leds_2 = num_leds_2 + random_num_2;
    num_leds_3 = num_leds_3 + random_num_3;
    num_leds_4 = num_leds_4 + random_num_4;
    num_leds_5 = num_leds_5 + random_num_5;
    num_leds_6 = num_leds_6 + random_num_6;
    num_leds_7 = num_leds_7 + random_num_7;
    num_leds_8 = num_leds_8 + random_num_8;
    num_leds_9 = num_leds_9 + random_num_9;
    num_leds_10 = num_leds_10 + random_num_10;
    num_leds_11 = num_leds_11 + random_num_11;
    num_leds_12 = num_leds_12 + random_num_12;
    delay(10);
    FastLED.show();
    for (int i = 0; i < 10; i++){
      fadeToBlackBy(leds, NUM_LEDS, 10);
      fadeToBlackBy(leds2, NUM_LEDS, 10);
      fadeToBlackBy(leds3, NUM_LEDS, 10);
      fadeToBlackBy(leds4, NUM_LEDS, 10);
      fadeToBlackBy(leds5, NUM_LEDS, 10);
      fadeToBlackBy(leds6, NUM_LEDS, 10);
      fadeToBlackBy(leds7, NUM_LEDS, 10);
      fadeToBlackBy(leds8, NUM_LEDS, 10);
      fadeToBlackBy(leds9, NUM_LEDS, 10);
      fadeToBlackBy(leds10, NUM_LEDS, 10);
      fadeToBlackBy(leds11, NUM_LEDS, 10);
      fadeToBlackBy(leds12, NUM_LEDS, 10);
      delay(5);
      FastLED.show();
    }
  }
}

void cascade(){
    fill_solid(leds, 99,CRGB(200,100,20));
    fill_solid(leds2, 99,CRGB(200,100,20));
    fill_solid(leds3, 99,CRGB(200,100,20));
    fill_solid(leds4, 99,CRGB(200,100,20));
    fill_solid(leds5, 99,CRGB(200,100,20));
    fill_solid(leds6, 99,CRGB(200,100,20));
    fill_solid(leds7, 99,CRGB(200,100,20));
    fill_solid(leds8, 99,CRGB(200,100,20));
    fill_solid(leds9, 99,CRGB(200,100,20));
    fill_solid(leds10, 99,CRGB(200,100,20));
    fill_solid(leds11, 99,CRGB(200,100,20));
    fill_solid(leds12, 99,CRGB(200,100,20));
    CRGB wave_colour = CRGB(245,80,50);
    for (int i = 0; i < 300; i++){
    //fill_solid(leds, NUM_LEDS, CRGB(0,40,40));
    //fill_solid(leds2, NUM_LEDS, CRGB(0,40,40));
      uint16_t sinBeat = beatsin16(10, 0, NUM_LEDS - 1, 0, 0);
      if ((constrain(sinBeat+2,0,99) == 99) || (constrain(sinBeat,0,99) == 0)){
        wave_colour = CRGB(100,2,10);
      }

      leds[constrain(sinBeat,0,99)] = wave_colour;
      leds[constrain(sinBeat+1,0,99)] = wave_colour;
      leds[constrain(sinBeat+2,0,99)] = wave_colour;
      leds2[constrain(sinBeat,0,99)] = wave_colour;
      leds2[constrain(sinBeat+1,0,99)] = wave_colour;
      leds2[constrain(sinBeat+2,0,99)] = wave_colour;
      leds3[constrain(sinBeat,0,99)] = wave_colour;
      leds3[constrain(sinBeat+1,0,99)] = wave_colour;
      leds3[constrain(sinBeat+2,0,99)] = wave_colour;
      leds4[constrain(sinBeat,0,99)] = wave_colour;
      leds4[constrain(sinBeat+1,0,99)] = wave_colour;
      leds4[constrain(sinBeat+2,0,99)] = wave_colour;
      leds5[constrain(sinBeat,0,99)] = wave_colour;
      leds5[constrain(sinBeat+1,0,99)] = wave_colour;
      leds5[constrain(sinBeat+2,0,99)] = wave_colour;
      leds6[constrain(sinBeat,0,99)] = wave_colour;
      leds6[constrain(sinBeat+1,0,99)] = wave_colour;
      leds6[constrain(sinBeat+2,0,99)] = wave_colour;
      leds7[constrain(sinBeat,0,99)] = wave_colour;
      leds7[constrain(sinBeat+1,0,99)] = wave_colour;
      leds7[constrain(sinBeat+2,0,99)] = wave_colour;
      leds8[constrain(sinBeat,0,99)] = wave_colour;
      leds8[constrain(sinBeat+1,0,99)] = wave_colour;
      leds8[constrain(sinBeat+2,0,99)] = wave_colour;
      leds9[constrain(sinBeat,0,99)] = wave_colour;
      leds9[constrain(sinBeat+1,0,99)] = wave_colour;
      leds9[constrain(sinBeat+2,0,99)] = wave_colour;
      leds10[constrain(sinBeat,0,99)] = wave_colour;
      leds10[constrain(sinBeat+1,0,99)] = wave_colour;
      leds10[constrain(sinBeat+2,0,99)] = wave_colour;
      leds11[constrain(sinBeat,0,99)] = wave_colour;
      leds11[constrain(sinBeat+1,0,99)] = wave_colour;
      leds11[constrain(sinBeat+2,0,99)] = wave_colour;
      leds12[constrain(sinBeat,0,99)] = wave_colour;
      leds12[constrain(sinBeat+1,0,99)] = wave_colour;
      leds12[constrain(sinBeat+2,0,99)] = wave_colour;
      delay(20);
      FastLED.show();
  }
}

void galaxy(){
  uint8_t indexgal = 255;
  for (int y = 0; y < 100; y++){
    for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = ColorFromPalette(galaxyPal2,indexgal-i); //constrain(255-i,180,255));
        leds2[i] = ColorFromPalette(galaxyPal2, indexgal-i); //constrain(255-i,180,255));
        leds3[i] = ColorFromPalette(galaxyPal2,indexgal-i); //constrain(255-i,180,255));
        leds4[i] = ColorFromPalette(galaxyPal2, indexgal-i); //constrain(255-i,180,255));
        leds5[i] = ColorFromPalette(galaxyPal2,indexgal-i); //constrain(255-i,180,255));
        leds6[i] = ColorFromPalette(galaxyPal2, indexgal-i); //constrain(255-i,180,255));
        leds7[i] = ColorFromPalette(galaxyPal2,indexgal-i); //constrain(255-i,180,255));
        leds8[i] = ColorFromPalette(galaxyPal2, indexgal-i); //constrain(255-i,180,255));
        leds9[i] = ColorFromPalette(galaxyPal2,indexgal-i); //constrain(255-i,180,255));
        leds10[i] = ColorFromPalette(galaxyPal2, indexgal-i); //constrain(255-i,180,255));
        leds11[i] = ColorFromPalette(galaxyPal2,indexgal-i); //constrain(255-i,180,255));
        leds12[i] = ColorFromPalette(galaxyPal2, indexgal-i); //constrain(255-i,180,255));
    }
    for (int x = 0; x < 20; x++){
      int random_int =  random(0,99);
      leds[random_int] = CRGB::FairyLight; //constrain(255-i,180,255));
      leds2[random_int]= CRGB::FairyLight;
      leds3[random_int] = CRGB::FairyLight; //constrain(255-i,180,255));
      leds4[random_int]= CRGB::FairyLight;
      leds5[random_int] = CRGB::FairyLight; //constrain(255-i,180,255));
      leds6[random_int]= CRGB::FairyLight;
      leds7[random_int] = CRGB::FairyLight; //constrain(255-i,180,255));
      leds8[random_int]= CRGB::FairyLight;
      leds9[random_int] = CRGB::FairyLight; //constrain(255-i,180,255));
      leds10[random_int]= CRGB::FairyLight;
      leds11[random_int] = CRGB::FairyLight; //constrain(255-i,180,255));
      leds12[random_int]= CRGB::FairyLight;
      //fadeToBlackBy(leds, NUM_LEDS, 1);
      //fadeToBlackBy(leds2, NUM_LEDS, 1);
    }
    delay(40);
    FastLED.show();

  }
}

void rainbowDot(){
  fill_rainbow(leds,NUM_LEDS,5);
  fill_rainbow(leds2,NUM_LEDS,5);
  fill_rainbow(leds3,NUM_LEDS,5);
  fill_rainbow(leds4,NUM_LEDS,5);
  fill_rainbow(leds5,NUM_LEDS,5);
  fill_rainbow(leds6,NUM_LEDS,5);
  fill_rainbow(leds7,NUM_LEDS,5);
  fill_rainbow(leds8,NUM_LEDS,5);
  fill_rainbow(leds9,NUM_LEDS,5);
  fill_rainbow(leds10,NUM_LEDS,5);
  fill_rainbow(leds11,NUM_LEDS,5);
  fill_rainbow(leds12,NUM_LEDS,5);
  for (int y = 0; y < NUM_LEDS*3; y++){
    uint8_t psawPos = map(255 - beat8(speed_array[4], 0), 0, 255, 0, NUM_LEDS - 1);
    uint8_t osawPos = map(255 - beat8(speed_array[4], 0), 0, 255, 0, NUM_LEDS - 1);
    for (int i = 0; i < NUM_LEDS; i++){
      leds[psawPos]   = CRGB(245,245,245);
      leds2[osawPos]   = CRGB(245,245,245);
      leds3[psawPos]   = CRGB(245,245,245);
      leds4[osawPos]   = CRGB(245,245,245);
      leds5[psawPos]   = CRGB(245,245,245);
      leds6[osawPos]   = CRGB(245,245,245);
      leds7[psawPos]   = CRGB(245,245,245);
      leds8[osawPos]   = CRGB(245,245,245);
      leds9[psawPos]   = CRGB(245,245,245);
      leds10[osawPos]   = CRGB(245,245,245);
      leds11[psawPos]   = CRGB(245,245,245);
      leds12[osawPos]   = CRGB(245,245,245);
    }
    delay(10);
    FastLED.show();
    fill_rainbow(leds,NUM_LEDS,5);
    fill_rainbow(leds2,NUM_LEDS,5);
    fill_rainbow(leds3,NUM_LEDS,5);
    fill_rainbow(leds4,NUM_LEDS,5);
    fill_rainbow(leds5,NUM_LEDS,5);
    fill_rainbow(leds6,NUM_LEDS,5);
    fill_rainbow(leds7,NUM_LEDS,5);
    fill_rainbow(leds8,NUM_LEDS,5);
    fill_rainbow(leds9,NUM_LEDS,5);
    fill_rainbow(leds10,NUM_LEDS,5);
    fill_rainbow(leds11,NUM_LEDS,5);
    fill_rainbow(leds12,NUM_LEDS,5);
    delay(2);
    FastLED.show();
  }
}
// Passive patterns
//void lightsFlickering(){
//  leds1

/*
void halfSineWave(){
  for (int i = 0; i < NUM_LEDS*2; i++){
    fill_solid(leds, NUM_LEDS, CRGB(0,40,40));
    fill_solid(leds2, NUM_LEDS, CRGB(0,40,40));
    uint16_t sinBeat = beatsin8(10, 0, 50, 0, 0);
    uint16_t cosBeat2 = beatcos8(10, 50, NUM_LEDS-1, 0, 0);
    leds[sinBeat] = CRGB::Purple;
    leds2[cosBeat2]= CRGB::Purple;
    leds[sinBeat] = CRGB::Purple;
    leds[constrain(sinBeat+1,0,50)] = CRGB::Purple;
    leds[constrain(sinBeat2+50,50,99)] = CRGB::Purple;
    leds[constrain(sinBeat2+51,50,99)] = CRGB::Purple;
    leds2[sinBeat] = CRGB::Purple;
    leds2[constrain(sinBeat+1,0,50)] = CRGB::Purple;
    leds2[constrain(sinBeat2+50,50,99)] = CRGB::Purple;
    leds2[constrain(sinBeat2+51,50,99)] = CRGB::Purple;
  
    fadeToBlackBy(leds, NUM_LEDS, 10);
    fadeToBlackBy(leds2, NUM_LEDS, 10);
    FastLED.show();
    delay(20);
  }
}
*/

