// COMM 4 - Current PC Conection for Ref

/*
 * OCT 2021 VERSION 1.1 - TESTED WORKING
DCS BACKLIGHTING CODE BY BEN MELROSE
TO CONTROL OPEN HORNET WS2812B LED BACK LIGHT PANELS BY L.WALKER
CODE IS CURRENTLY WRITTEN FOR 3 CHANNELS OF BACKLIGHTING
CODE WILL BE UPDATED TO A FULL PIT CODE DURING TESTING
NVD / DAY / NIGHT SWITCH ADDED

 */

#include "FastLED.h"
#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
//
#define NUM_LEDS_CHA 78 // Open Hornet SPIN RCVRY Backlight PCB Version 2.0 by L Walker
#define DATA_PIN_CHA 12// ARDUINO PIN that SPIN RCVRY is connected to
//
#define NUM_LEDS_CHB 400 // Generic Backlight loop - Left Console (400 LEDS) Change as required
#define DATA_PIN_CHB 8// ARDUINO PIN that Left Console is connected to
//
#define NUM_LEDS_CHC 400 // Generic Backlight loop - Right Console (400 LEDS) Change as required
#define DATA_PIN_CHC 9 // ARDUINO PIN that Right Console is connected to
//
#define NUM_LEDS_CHD 78 // Generic Backlight loop - UIP (78 LEDS) Change as required
#define DATA_PIN_CHD 11 // ARDUINO PIN that Right Console is connected to
//
//#define NUM_LEDS_CHE 100 // Generic Backlight loop - LIP (200 LEDS) Change as required
//#define DATA_PIN_CHE 10 // ARDUINO PIN that LIP is connected to
//
#define NUM_LEDS_CHF 200 // Generic Backlight loop - SPARE (100 LEDS) Change as required
#define DATA_PIN_CHF 13 // ARDUINO PIN that SPARE is connected to

#define LED_TYPE    WS2812B

// Define the array of leds relating to SPIN RCVRY Backlight PCB Version 2.0 by L Walker
// GREEN BACKLIGHT LED NUMBERS
int spinPanel[ ] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 75, 77};

// GREEN BACKLIGHT LED NUMBERS
int emcJettPanel[ ] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 75, 77};

int i; //General int
int j; // INT used for JETT Button
int k;
int l;
int m;


int instDim; // Consoles Dimmer Knob Value - Via DCS
int conDim;
int cautDim; // Caution Dimmer Knob Value - Via DCS
int spinLT; //Spin Light Dimmer Value
int spinOn; // Spin Light On or Off
int consSW; // NVG/NITE/DAY Switch
int emcLT; // EMC JETT LIGHT (GREEN)
int emcON; // EMC Light ON (GREEN)

CRGB leds_A[NUM_LEDS_CHA];
CRGB leds_B[NUM_LEDS_CHB];
CRGB leds_C[NUM_LEDS_CHC];
CRGB leds_D[NUM_LEDS_CHD];
//CRGB leds_E[NUM_LEDS_CHE];
+CRGB leds_F[NUM_LEDS_CHF];
/// ADD EXTRA CHANNELS AS REQUIRED

void onCockkpitLightModeSwChange(unsigned int newValue) {
consSW = newValue; // (0, 1, 2)
}
DcsBios::IntegerBuffer cockkpitLightModeSwBuffer(0x74c8, 0x0600, 9, onCockkpitLightModeSwChange);

void onConsolesDimmerChange(unsigned int newValue) {
  if (consSW == 2) {
instDim = 1;
}
  else {
instDim = map(newValue, 0, 65535, 0, 220);
  }
// MAP The Concole DIM Value from DCS to 0 - 150 ## NOTE MAX Can Be 255 ##
}
DcsBios::IntegerBuffer consolesDimmerBuffer(0x7544, 0xffff, 0, onConsolesDimmerChange);

void onInstPnlDimmerChange(unsigned int newValue) {
  if (consSW == 2) {
conDim = 1;
}
  else {
conDim = map(newValue, 0, 65535, 0, 220);
}
}
DcsBios::IntegerBuffer instPnlDimmerBuffer(0x7546, 0xffff, 0, onInstPnlDimmerChange);


void onWarnCautionDimmerChange(unsigned int newValue) {
cautDim = map(newValue, 0, 65535, 20, 150);
// MAP The Caution Lights DIM Value from DCS to 0 - 150 ## NOTE MAX Can Be 255 ##
}
DcsBios::IntegerBuffer warnCautionDimmerBuffer(0x754c, 0xffff, 0, onWarnCautionDimmerChange);

void onSpinLtChange(unsigned int newValue) {
if (newValue == 1) {
  spinLT = cautDim; // Set Brightness of "SPIN" Light LEDS
  spinOn = 1; // SET the "SPIN" Light (ON)
  }
  else {
    spinLT = 0; // Set Brightness of "SPIN" Light LEDS to 0
    spinOn = 0; // SET the "SPIN" Light (OFF)
        }
  }
DcsBios::IntegerBuffer spinLtBuffer(0x742a, 0x0800, 11, onSpinLtChange);

void onCmsdJetSelLChange(unsigned int newValue) {
if (newValue == 1) {
  emcLT = cautDim; // Set Brightness of "SPIN" Light LEDS
  emcON = 1; // SET the "SPIN" Light (ON)
  }
  else {
    emcON = 0; // Set Brightness of "SEL/JETT" Light LEDS to 0
    emcLT = 0; // SET the "SEL/JETT" Light (OFF)
        }
  }
DcsBios::IntegerBuffer cmsdJetSelLBuffer(0x74d6, 0x8000, 15, onCmsdJetSelLChange);


void setup() 
{ 
 
FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);   
FastLED.addLeds<WS2812B, DATA_PIN_CHC, RGB>(leds_C, NUM_LEDS_CHC);    
FastLED.addLeds<WS2812B, DATA_PIN_CHD, RGB>(leds_D, NUM_LEDS_CHD);    
//FastLED.addLeds<WS2812B, DATA_PIN_CHE, RGB>(leds_E, NUM_LEDS_CHE);    // ADD IF REQUIRED
FastLED.addLeds<WS2812B, DATA_PIN_CHF, RGB>(leds_F, NUM_LEDS_CHF);    // ADD IF REQUIRED

//TEST ALL LEDS -RED-BLUE-BLACK-GREEN
   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_C, NUM_LEDS_CHC, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_D, NUM_LEDS_CHD, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_F, NUM_LEDS_CHF, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
    FastLED.setBrightness(50); // SET CONSOLE PANELS GREEN BRIGHTNESS = TEST ALL GREEN
    FastLED.show();
    delay(500);
   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Green); // Set all Console LEDs to ##RED##. 
   fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Green); // Set all Console LEDs to ##RED##. 
   fill_solid(leds_C, NUM_LEDS_CHC, CRGB::Green); // Set all Console LEDs to ##RED##. 
   fill_solid(leds_D, NUM_LEDS_CHD, CRGB::Green); // Set all Console LEDs to ##RED##. 
    FastLED.setBrightness(50); // SET CONSOLE PANELS RED BRIGHTNESS - TEST ALL RED
    FastLED.show();
    delay(500);
   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Blue); // Set all Console LEDs to ##BLUE##. 
   fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Blue); // Set all Console LEDs to ##BLUE##. 
   fill_solid(leds_C, NUM_LEDS_CHC, CRGB::Blue); // Set all Console LEDs to ##BLUE##. 
   fill_solid(leds_D, NUM_LEDS_CHD, CRGB::Blue); // Set all Console LEDs to ##BLUE##. 
    FastLED.setBrightness(50); // SET CONSOLE PANELS BLUE BRIGHTNESS - TEST ALL BLUE
    FastLED.show();
    delay(500);
     FastLED.clear(); // CLEAR ALL LEDS AFTER TEST
     FastLED.show();
 
  DcsBios::setup();   
}

void loop() {
     DcsBios::loop();
{
      static int prevInstDim;
  if (instDim != prevInstDim)
  {
    prevInstDim = instDim;
    // CONSOLES DIMMERS
     for (k = 0; k < NUM_LEDS_CHB; k++) 
    leds_B[k] = CHSV( 0, 255, instDim); // SET CONSOLE GREEN BRIGHTNESS
     for (l = 0; l < NUM_LEDS_CHC; l++) 
    leds_C[l] = CHSV( 0, 255, instDim); // SET CONSOLE GREEN BRIGHTNESS
     
   FastLED.show();
  }
}
{
        static int prevConDim;
  if (conDim != prevConDim)
  {
    prevConDim = conDim;
    for (i = 0; i < 50; i++) 
    leds_A[spinPanel[i]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS
        for (j = 0; j < 78; j++) 
    leds_D[emcJettPanel[j]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS
    FastLED.show();
  }
}
    // SET SPIN RED LEDS to FLASH via DCS OUTPUT
        static int SpinOn;
  if (spinOn != SpinOn)
  {
    SpinOn = spinOn;
  leds_A[29] = CHSV( 96, 255, spinLT); // SPIN PANEL LED 29 "96 = RED"
  leds_A[36] = CHSV( 96, 255, spinLT); // SPIN PANEL LED 36 "96 = RED"
    FastLED.show();
  }
          static int EmcON;
  if (emcON != EmcON)
  {
    EmcON = emcON;
  leds_D[74] = CHSV( 0, 255, emcLT); // EMC LED 75 "96 = RED"
  leds_D[76] = CHSV( 0, 255, emcLT); // EMC LED 77 "96 = RED"
    FastLED.show();
  }
  
}

//END
