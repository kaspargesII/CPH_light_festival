/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino
#include <cstdlib>  // Include for older compilers
#include <ctime>    // Include for seeding
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
#define DATA_PIN_ONE 22
#define DATA_PIN_TWO 23

HX711 scale;
float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup

//#define CLOCK_PIN 13
// Time variables

unsigned long timer = 0;
unsigned long jump_timer = 0;
int flicker_delay = 100;
int flicker_timer = 0;
int interval = 200;
int jump_interval = 300;
int jump_array1[NUM_LEDS];
int jump_array2[NUM_LEDS];
unsigned long fade_timer = 0;
int fade_interval = 50;
int standing_weight_interval = 50;
float weight_array[4] = {0,0,0,0};
int counter = 0;
bool standing = false;
float sum = 0;
int weight_counter = 0;
float person_weight = 0;
// Define the array of leds
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
int rain_array1[NUM_LEDS];
int rain_array2[NUM_LEDS];
int flicker_array1[NUM_LEDS];
int flicker_array2[NUM_LEDS];

//CRGB LEDS[TotalLeds];

void setup() { 
    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned>(std::time(0)));
    // LED setup
    FastLED.addLeds<WS2811, DATA_PIN_ONE, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<WS2811, DATA_PIN_TWO, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );
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


void update_rain(int rain_array[]) {
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
        if (rain_array[i] == 1) {
            rain_array[i] = 0;
            if (i < NUM_LEDS - 1 && random(100) < 50) {
                rain_array[i + 1] = 1;
            }
        }
    }
}

// STELA'S Version --> Adds random falling time + makes a tail for the raindrop
/*
void update_rain(int rain_array[]) {
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
        if (rain_array[i] == 1) {
            rain_array[i] = 0;
            if (i < NUM_LEDS - 1 && std::rand() % 100 < 50) {
                rain_array[i + 1] = 1;
            }
        }
    }
}
*/
/*
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
}*/
void show_rain(int rain_array[], CRGB led_strip[]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (rain_array[i] == 1) {
            // Use a constant raindrop color
            CRGB raindropColor = CRGB(51, 33, 0);
            // Randomize brightness for variation
            int brightness = random(50, 255);
            // Randomize tail length
            int tailLength = random(1, 6); // Adjust the range of tail lengths
            // Display raindrop and fading trail with tail
            for (int j = i; j < min(NUM_LEDS, i + tailLength + 5); j++) {
                int trailBrightness = map(j, i, min(NUM_LEDS, i + tailLength + 5), 10, 255);
                led_strip[j] = raindropColor.fadeToBlackBy(trailBrightness);
            }
        } else {
            led_strip[i] = CRGB(72, 61, 67);
        }
    }
  
  int sum = 0;
  for (int i =0; i< random(5,10);i++){
    sum = sum + rain_array[i];
    }
  if (sum == 0){
    rain_array[0] = 1;
  }
  else {
    int newDropletPosition = random(7, NUM_LEDS);
    rain_array[newDropletPosition] = 1;
  }
}

/*
// STELA'S Version --> Adds random drop appearance + makes a tail for the raindrop
void show_rain(int rain_array[], CRGB led_strip[]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (rain_array[i] == 1) {
            // Randomize raindrop appearance
            CRGB raindropColor = CRGB(220, 220, 220);
            int raindropLength = std::rand() % 7 + 3;  // Random length between 3 and 10
            int brightness = std::rand() % 206 + 50;   // Random brightness between 50 and 255

            for (int j = i; j < min(NUM_LEDS, i + raindropLength); j++) {
                int trailBrightness = map(j, i, min(NUM_LEDS, i + 5), 10, 255);
                led_strip[j] = raindropColor.fadeToBlackBy(trailBrightness);
            }
        } else {
            led_strip[i] = CRGB(10, 0, 10);
        }
    }
}*/
void fade_rain(int rain_array[], CRGB led_strip[]){
  // Fades a rain drops brightness
  for( int i = 0; i < NUM_LEDS; i++){
    if (rain_array[i]==1){
      led_strip[i]/=1.5;
    }
  }
}

void update_jump(float force,int jump_array[]){
  for( int i = 0; i< force; i++){
    if (jump_array[i]==1){
      if (i < NUM_LEDS-1){
        jump_array[i+1]=1;
    }
  }
}
}
void show_jump(int jump_array[], CRGB led_strip[]){
  for( int i = 0; i < NUM_LEDS; i++){
    if (jump_array[i]==1){
      led_strip[i] = CRGB(0,180,70);
    }
    else{
      led_strip[i] = CRGB(10,0,10);
    }
  }
  jump_array[0] = 1;
}

int convert_float(float x){
    int y = static_cast<int>(x);
    std::cout << y << std::endl; //Prints out 3
    return y;
}
void jump(float force, CRGB led_strip1[], CRGB led_strip2[]){
    int colour1 =0;
    int colour2 =170;
    int colour3 =130;
    int range = convert_float(force);
    for( int i = 0; i < constrain(0,(int) range*2,100); i++){
        led_strip1[i] = CRGB(colour1,colour2,colour3);
        led_strip2[i] = CRGB(colour1,colour2,colour3);
        delay(20+2*i);
        FastLED.show();
      }
    }

//}
void stand(float force, CRGB led_strip[]){
    for( int i = 0; i < constrain(0,(int) force,100); i++){
      led_strip[i] = CRGB(255,0,0);
  }

}

int printing(float force){
  cout << "Force on weight: " << force << endl;
  return 0;
}

void flicker_lights(CRGB led_strip[]){
  for( int i = 0; i < constrain(0,NUM_LEDS,99); i = i+2){
      long rand_numb = random(0,1);
      if ((rand_numb > 0.35) && (rand_numb > 0.75)){
        led_strip[i] = CRGB(220,0,0);
        led_strip[i+1] = CRGB(220,0,0);
      }
      else if (rand_numb > 0.35){
        led_strip[i] = CRGB(220,0,0);
      }
      else{
        led_strip[i] = CRGB(10,0,10);
      }
    FastLED.show();
  }

}

void loop(){
  // SCALE STUFF
  if (counter <= 3){
    counter = counter + 1;
  }
  if (counter == 5){
    counter = 0;
  }
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading:");
  float force = scale.get_units();
  Serial.print("\n");
  Serial.print(scale.get_units(), 1);
  //Serial.print(" lbs"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  //Serial.print(" calibration_factor: ");
  //Serial.print(calibration_factor);
  //Serial.println();
  //printing(force);
  if (scale.get_units() > 10){
    // Trigger standing
    standing = true;
    Serial.print("Standing:");
    Serial.print(scale.get_units());
    bool not_weighted = true;
    float weight_array[20];
    float idle_array[10];
    float avg_weight = 0;
    //if (millis() > flicker_timer + flicker_delay){
    //flicker_lights(leds1);
    //flicker_lights(leds2);
    //flicker_timer = millis();
    // }
    while (standing){
      if (not_weighted){
        // Weigh the person when they stand on the scale
         for( int i = 0; i < 19; i++){
          Serial.print("Standing:");
          Serial.print("\n");
          float time_weight = scale.get_units();
          weight_array[i] = time_weight;
          Serial.print(time_weight);
          if (i == 18){
          not_weighted = false;
            }
          }
        }else{
      // After the weighing
      // Calculate their weight, hopefully GDPR compatible
      float person_weight = (weight_array[8] + weight_array[10] + weight_array[12] +weight_array[14] +weight_array[16] +weight_array[18])/6;
      // retrieve changes in idle array
      for( int i = 0; i < 10; i++){
        float idle_time_weight = scale.get_units();
        idle_array[i] = idle_time_weight;
      }
      float idle_weight = (idle_array[0]+idle_array[1]+idle_array[2]+idle_array[3]+idle_array[4]+idle_array[5]+idle_array[6]+idle_array[7]+idle_array[8]+idle_array[9])/10;
      Serial.print("PERSON WEIGHT");
      Serial.print(person_weight);
      Serial.print("\n");
      Serial.print("IDLE WEIGHT");
      Serial.print(idle_weight);
      Serial.print("\n");

      // If the force on the weight is very different from their weight, it means they are jumping!!!!!
      if (idle_weight-person_weight > 10){
          /*if (millis() > jump_timer + jump_interval){ 
            //set_colour(leds1);
            update_jump(30,jump_array1);
            update_jump(30,jump_array2);
            show_jump(jump_array1, leds1);
            show_jump(jump_array2, leds2);
            FastLED.show();
            jump_timer = millis();
          }
          */
          //Serial.print(idle_weight-person_weight);
          jump(idle_weight-person_weight,leds1,leds2);
      }

      // We need to figure out if the person has left the weight; 
      if (idle_weight < 10){
        // If the weight goes down we detect that nobody is standing on the weight; might be problematic in the instant that the person is jumping
        standing = false;
        Serial.print("No longer standing:");
        Serial.print(scale.get_units());
      }
    }
  }
  }
    // Standing
    
    //while (scale.get_units() > 10){
      //scale.tare();
      //scale.set_scale(calibration_factor); //Adjust to this calibration factor
      //Serial.print("Reading: ");
      //float force = scale.get_units();
      //Serial.print(scale.get_units(), 1);
       //Reset the scale to 0
      //if (force > 50){
      //  stand(force, leds1);
      //  stand(force, leds2);
      //  FastLED.show();
      //  delay(50);
        //scale.tare();
      //}
    
  //set_colour(leds1);

  // Timing of raindrops falling
  if ((millis() > fade_timer + fade_interval) && (millis() < timer + interval)) {
    fade_rain(rain_array1,leds1);
    fade_rain(rain_array2,leds2);
    fade_timer = millis();
    FastLED.show();
  }

  if (millis() > timer + interval){ 
    //set_colour(leds1);
    update_rain(rain_array1);
    update_rain(rain_array2);
    show_rain(rain_array1, leds1);
    show_rain(rain_array2, leds2);
    FastLED.show();
    timer = millis();
  }
}
}
