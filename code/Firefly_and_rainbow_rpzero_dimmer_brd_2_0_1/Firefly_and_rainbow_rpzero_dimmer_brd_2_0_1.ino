#include <Arduino.h>
#include <Wire.h>
//#include <SPI.h>
#include "Adafruit_NeoPixel.h"
#include <FastLED.h>

/************************************************************************************************************
/  Setting the following variables will define the lighting behavior of the fireflies
/************************************************************************************************************/

uint16_t LED_COUNT = 100;//300//97  // Set this to the number of LEDs in the string

//Set the number of lit fireflies possible at once.  This will be the number of rows in the matrix.
const uint16_t init_num_matrix_rows = 150;  // The number rows in the matrix (# of led slots)
uint16_t num_matrix_rows = 25;
unsigned int hue_inc = 3;
unsigned int modulo = 1; //floor(init_num_matrix_rows/7);

//Set led intensity rise and fall settings
int num_loops_in_rise = 50;  // Number of loops for both rise and fall.  50 recommended
float rise_exponent = 2.0; // Controls how fast of a curve the light increases in intensity. 2.0 recommended
int fall_exponent = 5; // Controls how fast of a curve the light increases in intensity. 5 recommended.  
int num_loops_at_max = 25;  // 25 recommended. the number of loops with LED fully on.
int delay_added_to_each_cycle = 11; // Controls the length of time to execute each loop. with 150 led string and 30 fireflies: 8 recommended for Pi Pico, 11 for Arduino Nano
/**********************************************************************************************************
/    End of user settings
/**********************************************************************************************************/

// Create variables
// The variables and values below are not user settings
// Just declaring some variables here.
#define LED_PIN 13 // Set this to the Arduino digital output pin you will use
unsigned int toggle_switch_pin = 8;
unsigned int jumper_pin = 15;
#define debounce 20 // debounce period in milli seconds

long loop_count = 0;
int  lit_led = 0;
bool move_firefly_flag = true;
bool current_toggle_status = LOW;
unsigned long start_time;
unsigned long loop_time;
unsigned int hue = 142;
float dim_val = 1.0;
CRGB rgb(255,255,0);
CHSV hsv;
unsigned int firefly_spawn_count = 0;
bool jumper_val;

int dimmerPin = 26;
int numffPin = 27;
int huePin = 28;
String config_type = "basic";

byte dip_pin_1 = 0;
byte dip_pin_2 = 1;
byte dip_pin_3 = 2;
byte dip_pin_4 = 3;
byte dip_pin_5 = 4;
byte dip_pin_6 = 5;
byte dip_pin_7 = 6;
byte dip_pin_8 = 7;
byte pinArray[] = {dip_pin_8,dip_pin_7,dip_pin_6,dip_pin_5,dip_pin_4,dip_pin_3,dip_pin_2,dip_pin_1};
//byte pinArray[] = {7,6,5,4,3,2,1,0};
byte orderByte;
byte numLedByte;
byte whiteByte;
byte colorByte;
byte routineByte;

byte rotary_pin_0 = 12;
byte rotary_pin_1 = 11;
byte rotary_pin_2 = 10;
byte rotary_pin_3 = 9;
byte rotary_pin_4 = 8;
byte rotary_pin_5 = 14;

byte rotaryPinArray[] = {rotary_pin_5,rotary_pin_4,rotary_pin_3,rotary_pin_2,rotary_pin_1,rotary_pin_0};
//byte rotaryPinArray[] = {12,11,10,9,8,14};
byte rotaryByte; 


// Create Matrix to hold the LED # of the up to 3 LEDs that can be lit
// at any particular time and the step in the rise/fall sequence of the LED intensit
// Rows correspond to an LED to turn on
// Columns are:
//   0) LED to be lit in the string
//   1) Illumination step LED is in
//   2) Counter of state when between rise and fall
//   3) loop counter, wraps around back to 0
long lit_Matrix[init_num_matrix_rows][7] { }; // Create and populate the matrix for lit fireflies.  A little wasteful since creates an oversize matrix rather than dynamic.  But simple.
unsigned short int rgb_permutations[] = {NEO_RGB, NEO_RBG, NEO_GRB, NEO_GBR, NEO_BRG, NEO_BGR};
uint16_t numberLeds[] = {50, 100, 150, 200};
Adafruit_NeoPixel strip  = Adafruit_NeoPixel(LED_COUNT, LED_PIN, rgb_permutations[2] + NEO_KHZ800);

void setup() {
  Serial.begin(9600); // Initialize ability to print to serial monitor
  //Wire.setSDA(4);   // Only needed if you want to use the Arduino logical pins to set SDA pin
  //Wire.setSCL(5);   // Only needed if you want to use the Arduino logical pins to set SDA pin
  delay(2000);
  Serial.println("This sketch is Firefly_and_rainbow_rpzero_dimmer_brd_1_0_1, version 20240725a");
  hsv = CHSV (0, 255, 255);
  //pinMode(toggle_switch_pin, INPUT_PULLUP);
  pinMode(jumper_pin, INPUT_PULLUP);
 
  pinMode(dip_pin_1, INPUT_PULLUP);
  pinMode(dip_pin_2, INPUT_PULLUP);
  pinMode(dip_pin_3, INPUT_PULLUP);
  pinMode(dip_pin_4, INPUT_PULLUP);
  pinMode(dip_pin_5, INPUT_PULLUP);
  pinMode(dip_pin_6, INPUT_PULLUP);
  pinMode(dip_pin_7, INPUT_PULLUP);
  pinMode(dip_pin_8, INPUT_PULLUP);

  pinMode(rotary_pin_0, INPUT_PULLUP);
  pinMode(rotary_pin_1, INPUT_PULLUP);
  pinMode(rotary_pin_2, INPUT_PULLUP);
  pinMode(rotary_pin_3, INPUT_PULLUP);
  pinMode(rotary_pin_4, INPUT_PULLUP);
  pinMode(rotary_pin_5, INPUT_PULLUP);

  jumper_val = digitalRead(jumper_pin);
  delay(5);
  jumper_val = digitalRead(jumper_pin);

  if (jumper_val == 1) {
    readDip();
    LED_COUNT = numberLeds[numLedByte];
    modulo =     (LED_COUNT/50);
    strip.updateLength(LED_COUNT);
    switch(numLedByte) {
      case 0:
          Serial.println("Case 0");
          delay_added_to_each_cycle = 12;
          num_matrix_rows = 16;
          break;
      case 1:
          Serial.println("Case 1");
          delay_added_to_each_cycle = 9;
          num_matrix_rows = 30;
          break;
      case 2:
          Serial.println("Case 2");
          delay_added_to_each_cycle = 8;
          num_matrix_rows = 38;
          break;     
      default:
          Serial.println("Case 3");
          delay_added_to_each_cycle = 7;
          num_matrix_rows = 50;
      }
  }
  strip.begin();
  strip.clear();
  // Light last LED as a test if all of string is working.
  strip.setPixelColor(LED_COUNT-1, 100, 0, 100);
  strip.show();
  delay(2000);
  strip.clear();
  strip.show();
} // End of Setup()

void loop() {
  if (loop_count%((2 * num_loops_in_rise) + num_loops_at_max) == 0) {  // Time complete loop for full firefly  
    loop_time = (millis() - start_time);
    //Serial.println(loop_time);
    start_time = millis();
  }
  for (unsigned int i=0; i < num_matrix_rows; i++) { //Let's light random fireflies
    if (lit_Matrix[i][0] >= 0) {
      if (loop_count == lit_Matrix[i][3]) {
        readDip();
        //Serial.print("orderByte: ");
        //Serial.println(orderByte);                                            
        strip.updateType(rgb_permutations[orderByte]);      
        getDimmerValue();        
        readRotary();
        setFromRotary(i);
      } 
      if (loop_count > lit_Matrix[i][3])  {
          if (lit_Matrix[i][1] >= 0) {
            if (lit_Matrix[i][1] < num_loops_in_rise)  {
              lit_Matrix[i][1] =  lit_Matrix[i][1] + 1;
            }
            else {
              lit_Matrix[i][1] = -lit_Matrix[i][1];
            }
            strip.setPixelColor(lit_Matrix[i][0], strip.Color(round(lit_Matrix[i][5]*pow(abs(lit_Matrix[i][1])/(float)num_loops_in_rise, rise_exponent)), round(lit_Matrix[i][6]*pow(abs(lit_Matrix[i][1])/(float)num_loops_in_rise, rise_exponent)), round(lit_Matrix[i][4]*pow(abs(lit_Matrix[i][1])/(float)num_loops_in_rise, rise_exponent))));
        }
        else {  // Not rising in illumination
          if (lit_Matrix[i][2] < num_loops_at_max && lit_Matrix[i][1] < -num_loops_in_rise + 1 ) {  // Stay at max illumination for num_loops_at_max number of loops
            lit_Matrix[i][2] =  lit_Matrix[i][2] + 1;
          }
          else  {  // illumination falls
            lit_Matrix[i][2] = 0;
            lit_Matrix[i][1] = lit_Matrix[i][1] + 1;
            if (lit_Matrix[i][1] == 0)
              move_firefly_flag = true;
          }
          strip.setPixelColor(lit_Matrix[i][0], strip.Color(round(lit_Matrix[i][5]*pow(abs(lit_Matrix[i][1])/(float)num_loops_in_rise, rise_exponent)), round(lit_Matrix[i][6]*pow(abs(lit_Matrix[i][1])/(float)num_loops_in_rise, rise_exponent)), round(lit_Matrix[i][4]*pow(abs(lit_Matrix[i][1])/(float)num_loops_in_rise, rise_exponent))));
        }
        lit_Matrix[i][3] = lit_Matrix[i][3] + 1;
        if (move_firefly_flag)
          move_firefly(i,lit_Matrix[i][0]);
      }
    }
  }
  strip.show();
  delay(delay_added_to_each_cycle);  // this delay increases the time for each loop to get fireflylight ing length to be ~1.7 seconds
  loop_count = loop_count + 1;   
}  // end of loop() (main loop)

void move_firefly(int led_slot, int lit_led) {
  move_firefly_flag = false;
  bool have_new_firefly = false;
  int new_firefly = random(0, LED_COUNT -1);  // get LED candidate not already lit
  while (have_new_firefly == false) {
    while ( new_firefly == lit_Matrix[led_slot][0] || new_firefly == lit_led )
      new_firefly = random(0, LED_COUNT -1);  // get LED candidate not already lit  
    have_new_firefly = true;
    //Serial.println(new_firefly);
    for (int j=0; j < num_matrix_rows; j++)  {
      if (lit_Matrix[led_slot][0] == new_firefly )
        have_new_firefly = false;
    } 
  }
  lit_Matrix[led_slot][0] = new_firefly;
  lit_Matrix[led_slot][1] = 0;
  lit_Matrix[led_slot][2] = 0;
  // logic to start a firefly very quickly if there are no fireflies lit, else select random # of loops to start next firefly
  int sum = 0;
  for ( int i = 0; i < num_matrix_rows - 1; i++ )  
  {
    sum += lit_Matrix[led_slot][3];
  }
  if (sum == 0)  {
    lit_Matrix[led_slot][3] = loop_count + 2;
  }
  else {
    lit_Matrix[led_slot][3] = loop_count + 50 + random(0, 600);
  }
}  // End of move_firefly()

void getDimmerValue() {
   // read the input on analog pin:                                                                                                                                                                                                                     
  analogRead(dimmerPin);
  delay(5);
  dim_val = float((1023 - analogRead(dimmerPin)))/1023.0;
  if (dim_val > .98) {
    dim_val = 1.0;
  }
  else if (dim_val < 0.0156862745) {
    dim_val = 0.011; // 0.015687;
  }
}  // End of getDimmerValues()

void getNumffValue() {
   // read the input on analog pin:                                                                                                                                                                                                                     
  analogRead(numffPin);
  delay(5);
  dim_val = float((1023 - analogRead(numffPin)))/1023.0;
  if (dim_val > .98) {
    dim_val = 1.0;
  }
  else if (dim_val < 0.0156862745) {
    dim_val = 0.011; // 0.015687;
  }
}  // End of getNumffValue()

void getHueValue() {
   // read the input on analog pin:                                                                                                                                                                                                                     
  analogRead(huePin);
  delay(5);
  hue = 255 * analogRead(huePin)/1023;     //(1023 - analogRead(huePin)/1023.0);
  Serial.print("Hue: ");
  Serial.println(hue);
}  // End of getHueValue()

void increment_hue() {
  if (hue < 256 - hue_inc) {
    hue = hue + hue_inc;
  }
  else {
    hue = 0;
  }
  Serial.print("Hue: ");
  Serial.println(hue);
}  // End of increment_hue()

void readRotary() {
  rotaryByte = 0;
  for (byte x=0; x<6; x=x+1){
    if(digitalRead(rotaryPinArray[x]) == LOW) { // test pin for electrical low that will be assigned as HIGH in next line
        rotaryByte = rotaryByte | (1<<x); //shift the bit into position and OR into its place in the data byte 
    }
  }
}

void setFromRotary(unsigned int i) {
  //Serial.println(rotaryByte);
  Serial.println(config_type);
  if (config_type == "basic") {
    switch(rotaryByte) {
      case 0:  // Yellow Firefly color
        Serial.println("Case 0 - Yellow Firefly");
        lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
        lit_Matrix[i][5] = ceil(255*dim_val); //Green for firefly color
        lit_Matrix[i][6] = 0; //Blue for firefly color
        break;
      case 1: // Cycle through rainbow
        Serial.println("Case 1 - Cycle through rainbow");
        hsv = CHSV (hue, 255, 255);
        hsv2rgb_rainbow (hsv, rgb);
        lit_Matrix[i][4] = ceil(rgb.red*dim_val);
        lit_Matrix[i][5] = ceil(rgb.green*dim_val);
        lit_Matrix[i][6] = ceil(rgb.blue*dim_val);
        if (modulo == 0) {
          increment_hue();
        }
        else if (firefly_spawn_count%modulo == modulo - 1) {
          increment_hue();
        }
        firefly_spawn_count = firefly_spawn_count + 1;  
        break;
      case 2:  // Warm white or dip switch choice of blue white
        Serial.println("Case 2 - Warm white or dip switch choice of blue white");
        readDip();
        getWhiteColor(i);
        break;          
      case 4: //Halloween orange with occasional purple
        Serial.println("Case 4 - Halloween orange with occasional purple");
        if (random(0,100) < 15)  {  // to randomly have skewed split between colors
          hue = 192;
          hsv = CHSV (hue, 255, 255);
          hsv2rgb_rainbow (hsv, rgb); 
          lit_Matrix[i][4] = ceil(rgb.red*dim_val);
          lit_Matrix[i][5] = ceil(rgb.green*dim_val);
          lit_Matrix[i][6] = ceil(rgb.blue*dim_val);  
        }
        else {
          hue = 17;
          hsv = CHSV (hue, 255, 255);
          hsv2rgb_rainbow (hsv, rgb); 
          lit_Matrix[i][4] = ceil(rgb.red*dim_val);
          lit_Matrix[i][5] = ceil(rgb.green*dim_val);
          lit_Matrix[i][6] = ceil(rgb.blue*dim_val);  
        }
        break;
      case 8:  // Blue with occasional green
        Serial.println("Case 8 - Blue with occasional green");
        if (random(0,100) < 15)  {  // to randomly have a 15/85 split between colors
            lit_Matrix[i][4] = 0; //Red for Christmas color
            lit_Matrix[i][5] = ceil(255*dim_val); //Green for ocassional color
            lit_Matrix[i][6] = 0; //Blue for firefly color
            }
        else {
          lit_Matrix[i][4] = 0; //Red
          lit_Matrix[i][5] = 0; //Green
          lit_Matrix[i][6] = ceil(255*dim_val); //Blue for main color
        }
        break;
      case 16:  // Grean with occasional blue
        Serial.println("Case 4 - Blue with occasional green");
        if (random(0,100) < 15)  {  // to randomly have a 15/85 split between colors
            lit_Matrix[i][4] = 0; //Red for Christmas color
            lit_Matrix[i][5] = 0; //Green for ocassional color
            lit_Matrix[i][6] = ceil(255*dim_val); //Blue for firefly color
            }
        else {
          lit_Matrix[i][4] = 0; //Red
          lit_Matrix[i][5] = ceil(255*dim_val); //Green
          lit_Matrix[i][6] = 0; //Blue for main color
        }
        break; 
      case 32: // Grean with occasional blue
        Serial.println("Case 32 - Green with occasional red");
        if (random(0,100) < 15)  {  // to randomly have a 15/85 split between colors
            lit_Matrix[i][4] = ceil(255*dim_val); //Red for Christmas color
            lit_Matrix[i][5] = 0; //Green for ocassional color
            lit_Matrix[i][6] = 0; //Blue for firefly color
            }
        else {
          lit_Matrix[i][4] = 0; //Red
          lit_Matrix[i][5] = ceil(255*dim_val); //Green
          lit_Matrix[i][6] = 0; //Blue for main color
        }
        break; 
      default:  // yellow firefly color
        Serial.println("Case: Default");
        lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
        lit_Matrix[i][5] = ceil(255*dim_val); //Green for firefly color
        lit_Matrix[i][6] = 0; //Blue for firefly color
        break;          
    }
  }
  else {
        switch(rotaryByte) {
      case 0:  // Yellow Firefly color
        Serial.println("Case 0 - Yellow Firefly");
        lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
        lit_Matrix[i][5] = ceil(255*dim_val); //Green for firefly color
        lit_Matrix[i][6] = 0; //Blue for firefly color
        break;
      case 1: // Hue controlled by Hue knob
        Serial.println("Case 1 - Hue controlled by Hue knob");
        getHueValue();
        hsv = CHSV (hue, 255, 255);
        hsv2rgb_rainbow (hsv, rgb); 
        lit_Matrix[i][4] = ceil(rgb.red*dim_val);
        lit_Matrix[i][5] = ceil(rgb.green*dim_val);
        lit_Matrix[i][6] = ceil(rgb.blue*dim_val);           
        break;
      case 2: // Cycle through rainbow
        Serial.println("Case 2 - Cycle through rainbow");
        hsv = CHSV (hue, 255, 255);
        hsv2rgb_rainbow (hsv, rgb);
        lit_Matrix[i][4] = ceil(rgb.red*dim_val);
        lit_Matrix[i][5] = ceil(rgb.green*dim_val);
        lit_Matrix[i][6] = ceil(rgb.blue*dim_val);
        if (modulo == 0) {
          increment_hue();
        }
        else if (firefly_spawn_count%modulo == modulo - 1) {
          increment_hue();
        }
        firefly_spawn_count = firefly_spawn_count + 1;  
        break;
      case 4:  // Warm white or dip switch choice of blue white
        Serial.println("Case 4 - Warm white or dip switch choice of blue white");
        readDip();
        getWhiteColor(i);
        //lit_Matrix[i][4] = ceil(255*dim_val); // Warm white is 255, 175, 40
        //lit_Matrix[i][5] = ceil(175*dim_val); //Green for firefly color
        //lit_Matrix[i][6] = ceil(40*dim_val); //Blue for firefly color
        break;          
      case 8:  // Blue with occasional green
        Serial.println("Case 8 - Blue with occasional green");
        if (random(0,100) < 15)  {  // to randomly have a 15/85 split between colors
            lit_Matrix[i][4] = 0; //Red for Christmas color
            lit_Matrix[i][5] = ceil(255*dim_val); //Green for ocassional color
            lit_Matrix[i][6] = 0; //Blue for firefly color
            }
        else {
          lit_Matrix[i][4] = 0; //Red
          lit_Matrix[i][5] = 0; //Green
          lit_Matrix[i][6] = ceil(255*dim_val); //Blue for main color
        }
        break; 
      case 16: //Halloween orange with occasional purple
        //Serial.println("Case 16 - Halloween orange with occasional purple");
        if (random(0,100) < 15)  {  // to randomly have skewed split between colors
          hue = 192;
          hsv = CHSV (hue, 255, 255);
          hsv2rgb_rainbow (hsv, rgb); 
          lit_Matrix[i][4] = ceil(rgb.red*dim_val);
          lit_Matrix[i][5] = ceil(rgb.green*dim_val);
          lit_Matrix[i][6] = ceil(rgb.blue*dim_val);  
        }
        else {
          hue = 17;
          hsv = CHSV (hue, 255, 255);
          hsv2rgb_rainbow (hsv, rgb); 
          lit_Matrix[i][4] = ceil(rgb.red*dim_val);
          lit_Matrix[i][5] = ceil(rgb.green*dim_val);
          lit_Matrix[i][6] = ceil(rgb.blue*dim_val);  
        }
        break; 
      case 32: //Hue from knob with second color from dip switch
        Serial.println("Case 16 - Hue with second color from dip switch");
        if (random(0,100) < 15)  {  // to randomly have skewed split between colors
        getHueValue();
        hsv = CHSV (hue, 255, 255);
        hsv2rgb_rainbow (hsv, rgb); 
        lit_Matrix[i][4] = ceil(rgb.red*dim_val);
        lit_Matrix[i][5] = ceil(rgb.green*dim_val);
        lit_Matrix[i][6] = ceil(rgb.blue*dim_val);           
        break;
        }
        else {  // still needs logic added for DIP awitch READ
          hue = 192;
          hsv = CHSV (hue, 255, 255);
          hsv2rgb_rainbow (hsv, rgb); 
          lit_Matrix[i][4] = ceil(rgb.red*dim_val);
          lit_Matrix[i][5] = ceil(rgb.green*dim_val);
          lit_Matrix[i][6] = ceil(rgb.blue*dim_val);  
        }
        break; 
      default:  // yellow firefly color
        Serial.println("Case: Default");
        lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
        lit_Matrix[i][5] = ceil(255*dim_val); //Green for firefly color
        lit_Matrix[i][6] = 0; //Blue for firefly color
        break;          
    }
  }
}

void getWhiteColor (unsigned int fireflyNumber)  {
  unsigned int i = fireflyNumber;
  //Serial.print("whiteByte: ");
  //Serial.println(whiteByte);
  switch(whiteByte) {
  case 0:
      //Serial.println("Case 0");
    lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
    lit_Matrix[i][5] = ceil(175*dim_val); //Green for firefly color
    lit_Matrix[i][6] = ceil(40*dim_val); //Blue for firefly color
      break;
  case 1:
    lit_Matrix[i][4] = ceil(200*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
    lit_Matrix[i][5] = ceil(255*dim_val); //Green for firefly color
    lit_Matrix[i][6] = ceil(255*dim_val); //Blue for firefly color
    break;
  default:
      lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
      lit_Matrix[i][5] = ceil(0*dim_val); //Green for firefly color
      lit_Matrix[i][6] = ceil(255*dim_val); //Blue for firefly color
  }
}

void readDip() {
  orderByte = 0;
  for (byte x=0; x<3; x=x+1){
    if(digitalRead(pinArray[x]) == LOW) { // test pin for electrical low that will be assigned as HIGH in next line
        orderByte = orderByte | (1<<x); //shift the bit into position and OR into its place in the data byte 
    }
  }
  numLedByte = 0;
  for (byte x=0; x<2; x=x+1){
    if(digitalRead(pinArray[x+3]) == LOW) { // test pin for electrical low that will be assigned as HIGH in next line
        numLedByte = numLedByte | (1<<x); //shift the bit into position and OR into its place in the data byte 
    }
  }
  whiteByte = 0;
  for (byte x=0; x<1; x=x+1){
    if(digitalRead(pinArray[x+5]) == LOW) { // test pin for electrical low that will be assigned as HIGH in next line
        whiteByte = whiteByte | (1<<x); //shift the bit into position and OR into its place in the data byte 
    }
  //Serial.print("whiteByte loop: ");
  //Serial.println(whiteByte);
  }
  colorByte = 0;
  for (byte x=0; x<2; x=x+1){
    if(digitalRead(pinArray[x+6]) == LOW) { // test pin for electrical low that will be assigned as HIGH in next line
        colorByte = colorByte | (1<<x); //shift the bit into position and OR into its place in the data byte 
    }
  Serial.print("colorByte loop: ");
  Serial.println(colorByte);
  }
  routineByte = 0;
  for (byte x=0; x<3; x=x+1){
    if(digitalRead(pinArray[x+5]) == LOW) { // test pin for electrical low that will be assigned as HIGH in next line
        routineByte = routineByte | (1<<x); //shift the bit into position and OR into its place in the data byte 
    }
  }
  Serial.print("routineByte: ");
  Serial.println(routineByte);
}

void get_dip_color (unsigned int fireflyNumber)  {
  unsigned int i = fireflyNumber;
  switch(routineByte) {
  case 0:
      //Serial.println("Case 0");
    lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
    lit_Matrix[i][5] = ceil(175*dim_val); //Green for firefly color
    lit_Matrix[i][6] = ceil(40*dim_val); //Blue for firefly color
      break;
  case 1:
    lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
    lit_Matrix[i][5] = ceil(210*dim_val); //Green for firefly color
    lit_Matrix[i][6] = ceil(100*dim_val); //Blue for firefly color
      break;
  case 2:
    lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
    lit_Matrix[i][5] = ceil(255*dim_val); //Green for firefly color
    lit_Matrix[i][6] = ceil(255*dim_val); //Blue for firefly color
    break;     
  case 3:
    // Do nothing as logic for this seting is handled in main loop to provide changing color through rainbow
    break;    
  case 4:
    lit_Matrix[i][4] = ceil(0*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
    lit_Matrix[i][5] = ceil(0*dim_val); //Green for firefly color
    lit_Matrix[i][6] = ceil(255*dim_val); //Blue for firefly color
    break;    
   case 5:
    lit_Matrix[i][4] = ceil(0*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
    lit_Matrix[i][5] = ceil(255*dim_val); //Green for firefly color
    lit_Matrix[i][6] = ceil(0*dim_val); //Blue for firefly color
    break;  
   case 6:
    lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
    lit_Matrix[i][5] = ceil(0*dim_val); //Green for firefly color
    lit_Matrix[i][6] = ceil(0*dim_val); //Blue for firefly color
    break;    
    default:
      lit_Matrix[i][4] = ceil(255*dim_val); //Red for firefly color  // Warm white is 255, 175, 40
      lit_Matrix[i][5] = ceil(0*dim_val); //Green for firefly color
      lit_Matrix[i][6] = ceil(255*dim_val); //Blue for firefly color
    }
}

bool read_toggle_switch() {  // legany code.  Not used in this sketch
  static long int elapse_time = 0;
  static bool transition_started = false;
  int pin_value;

  // test current state of toggle pin given switch to 0v rather than VCC
  pin_value = digitalRead(toggle_switch_pin);

  if (pin_value != current_toggle_status && !transition_started) {
    // Switch change detected so start debounce cycle
    //Serial.println("detected switch change");
    transition_started = true;
    elapse_time = millis();  // set start of debounce timer
  }
  else {
    if (transition_started) {
      // we are in the switch transition cycle so check if debounce period has elapsed
      if (millis() - elapse_time >= debounce) {  // debounce time
        // debounce period elapse  so assume switch has settled down after transition
        current_toggle_status  = !current_toggle_status;  // flip status
        for(int i=0;i<num_matrix_rows -1;i++) {
          for(int j=0;j<7;j++) {
            lit_Matrix[i][j] = 0; // Change all values in the array to 0. Technique only works for filling with zeros.
          }
        }
        transition_started = false;  // cease transition cycle
        strip.clear();
       }
    }
  }
  return current_toggle_status;
}  // End of read_toggle_switch()