/*
 * Alewife and Commuter Rail
*/

#include <Adafruit_NeoPixel.h>

/*
 * Pins, LED Counts, and Brightness
*/

#define BRIGHTNESS 20

// Red Line Alewife
#define RED_ALEWIFE_PIN 27
#define RED_ALEWIFE_LED_COUNT 20

// Commuter Rail
#define COMMUTER_PIN 33
#define COMMUTER_LED_COUNT 25

/*
 * Strip Definitions
*/

Adafruit_NeoPixel RedAlewife_strip = Adafruit_NeoPixel(RED_ALEWIFE_LED_COUNT, RED_ALEWIFE_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Commuter_strip = Adafruit_NeoPixel(COMMUTER_LED_COUNT, COMMUTER_PIN, NEO_GRB + NEO_KHZ800);

/*
 * Variables
*/

int alewife_index = 0;
int alewife_direction = 1;

int commuter_index = 0;
int commuter_direction = 1;

/*
 * Setup
*/

void setup() {
  RedAlewife_strip.begin();
  Commuter_strip.begin();

  RedAlewife_strip.setBrightness(BRIGHTNESS);
  Commuter_strip.setBrightness(BRIGHTNESS);
}

/*
 * Loop
*/

void loop() {
  // Alewife
  RedAlewife_strip.clear();
  RedAlewife_strip.setPixelColor(alewife_index, RedAlewife_strip.Color(255, 0, 0)); // Red
  RedAlewife_strip.show();
  if (alewife_index == RED_ALEWIFE_LED_COUNT - 1) {
    alewife_direction = -1;
  } else if (alewife_index == 0) {
    alewife_direction = 1;
  }
  alewife_index += alewife_direction;

  // Commuter Rail
  Commuter_strip.clear();
  Commuter_strip.setPixelColor(commuter_index, Commuter_strip.Color(255, 0, 255)); // Purple
  Commuter_strip.show();
  if (commuter_index == COMMUTER_LED_COUNT - 1) {
    commuter_direction = -1;
  } else if (commuter_index == 0) {
    commuter_direction = 1;
  }
  commuter_index += commuter_direction;

  delay(750);
}