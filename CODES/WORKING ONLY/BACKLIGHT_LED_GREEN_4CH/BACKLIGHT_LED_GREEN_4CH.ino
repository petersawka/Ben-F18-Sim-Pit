#include <FastLED.h>
#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#define NUM_LEDS_CHA 400
#define DATA_PIN_CHA 16

#define NUM_LEDS_CHB 400
#define DATA_PIN_CHB 17

#define LED_TYPE    WS2812B

#define MAX_BRIGHTNESS 200    // Thats full on, watch the power!
#define MIN_BRIGHTNESS 0       // set to a minimum of 25%
const int brightnessInPin = A4;  // The Analog input pin that the brightness control potentiometer is attached to.

// Define the array of leds
CRGB leds_A[NUM_LEDS_CHA];
CRGB leds_B[NUM_LEDS_CHB];

void setup() 
{ 
  FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);   


 // FastLED.addLeds(leds, NUM_LEDS); 
}

void loop() {

    green(); 

}
void green() {
 int mappedValue = map(analogRead(brightnessInPin), 30, 1023, 0, 255);
 FastLED.setBrightness(constrain(mappedValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all to ##GREEN##.
  fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all to ##GREEN##.

      FastLED.show();
}
