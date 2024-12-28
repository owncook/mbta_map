/*
 * Heath St, Riverside, Boston College, Braintree, Oak Grove, Medford, Forest Hills
*/

#include <Adafruit_NeoPixel.h>

/*
 * Pins, LED Counts, and Brightness
*/

#define BRIGHTNESS 20

// Green Line
#define GREENB_PIN 17
#define GREENB_LED_COUNT 18
#define GREENE_PIN 4
#define GREENE_LED_COUNT 10
#define GREEND_PIN 16
#define GREEND_LED_COUNT 13
#define GREENE2_PIN 5
#define GREENE2_LED_COUNT 15

// Red Line Braintree
#define RED_BRAINTREE_PIN 22
#define RED_BRAINTREE_LED_COUNT 22

// Orange Line
#define ORNG_FRST_PIN 21
#define ORNG_FRST_LED_COUNT 23
#define ORNG_OAK_PIN 18
#define ORNG_OAK_LED_COUNT 14

/*
 * Strip Definitions
*/

Adafruit_NeoPixel GreenB_strip = Adafruit_NeoPixel(GREENB_LED_COUNT, GREENB_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel GreenCandE_strip = Adafruit_NeoPixel(GREENE_LED_COUNT, GREENE_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel GreenD_strip = Adafruit_NeoPixel(GREEND_LED_COUNT, GREEND_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel GreenEMedford_strip = Adafruit_NeoPixel(GREENE2_LED_COUNT, GREENE2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel RedBraintree_strip = Adafruit_NeoPixel(RED_BRAINTREE_LED_COUNT, RED_BRAINTREE_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel OrangeForest_strip = Adafruit_NeoPixel(ORNG_FRST_LED_COUNT, ORNG_FRST_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel OrangeOakGrove_strip = Adafruit_NeoPixel(ORNG_OAK_LED_COUNT, ORNG_OAK_PIN, NEO_GRB + NEO_KHZ800);

/*
 * Variables
*/

int greenB_index = random(0, GREENB_LED_COUNT);
int greenB_direction = 1;

int greenE_index = random(0, GREENE_LED_COUNT);
int greenE_direction = 1;

int greenD_index = random(0, GREEND_LED_COUNT);
int greenD_direction = 1;

int greenE2_index = random(0, GREENE2_LED_COUNT);
int greenE2_direction = 1;

int braintree_index = random(RED_BRAINTREE_LED_COUNT);
int braintree_direction = 1;

int orange_frst_index = random(0, ORNG_FRST_LED_COUNT);
int orange_frst_direction = 1;

int orange_oak_index = random(0, ORNG_OAK_LED_COUNT);
int orange_oak_direction = 1;

/*
 * Setup
*/

void setup() {
  GreenB_strip.begin();
  GreenCandE_strip.begin();
  GreenD_strip.begin();
  GreenEMedford_strip.begin();
  RedBraintree_strip.begin();
  OrangeForest_strip.begin();
  OrangeOakGrove_strip.begin();

  GreenB_strip.setBrightness(BRIGHTNESS);
  GreenCandE_strip.setBrightness(BRIGHTNESS);
  GreenD_strip.setBrightness(BRIGHTNESS);
  GreenEMedford_strip.setBrightness(BRIGHTNESS);
  RedBraintree_strip.setBrightness(BRIGHTNESS);
  OrangeForest_strip.setBrightness(BRIGHTNESS);
  OrangeOakGrove_strip.setBrightness(BRIGHTNESS);
}

/*
 * Loop
*/

void loop() {
  // Green B
  GreenB_strip.clear();
  GreenB_strip.setPixelColor(greenB_index, GreenB_strip.Color(0, 255, 0));
  GreenB_strip.show();
  if (greenB_index == GREENB_LED_COUNT - 1) {
    greenB_direction = -1;
  } else if (greenB_index == 0) {
    greenB_direction = 1;
  }
  greenB_index += greenB_direction;

  // Green CE
  GreenCandE_strip.clear();
  GreenCandE_strip.setPixelColor(greenE_index, GreenCandE_strip.Color(0, 255, 0));
  GreenCandE_strip.show();
  if (greenE_index == GREENE_LED_COUNT - 1) {
    greenE_direction = -1;
  } else if (greenE_index == 0) {
    greenE_direction = 1;
  }
  greenE_index += greenE_direction;

  // Green D
  GreenD_strip.clear();
  GreenD_strip.setPixelColor(greenD_index, GreenD_strip.Color(0, 255, 0));
  GreenD_strip.show();
  if (greenD_index == GREEND_LED_COUNT - 1) {
    greenD_direction = -1;
  } else if (greenD_index == 0) {
    greenD_direction = 1;
  }
  greenD_index += greenD_direction;

  // Green E Medford
  GreenEMedford_strip.clear();
  GreenEMedford_strip.setPixelColor(greenE2_index, GreenEMedford_strip.Color(0, 255, 0));
  GreenEMedford_strip.show();
  if (greenE2_index == GREENE2_LED_COUNT - 1) {
    greenE2_direction = -1;
  } else if (greenE2_index == 0) {
    greenE2_direction = 1;
  }
  greenE2_index += greenE2_direction;

  // Braintree
  RedBraintree_strip.clear();
  RedBraintree_strip.setPixelColor(braintree_index, RedBraintree_strip.Color(255, 0, 0));
  RedBraintree_strip.show();
  if (braintree_index == RED_BRAINTREE_LED_COUNT - 1) {
    braintree_direction = -1;
  } else if (braintree_index == 0) {
    braintree_direction = 1;
  }
  braintree_index += braintree_direction;

  // Orange Forest Hills
  OrangeForest_strip.clear();
  OrangeForest_strip.setPixelColor(orange_frst_index, OrangeForest_strip.Color(255, 165, 0));
  OrangeForest_strip.show();
  if (orange_frst_index == ORNG_FRST_LED_COUNT - 1) {
    orange_frst_direction = -1;
  } else if (orange_frst_index == 0) {
    orange_frst_direction = 1;
  }
  orange_frst_index += orange_frst_direction;

  // Orange Oak Grove
  OrangeOakGrove_strip.clear();
  OrangeOakGrove_strip.setPixelColor(orange_oak_index, OrangeOakGrove_strip.Color(255, 165, 0));
  OrangeOakGrove_strip.show();
  if (orange_oak_index == ORNG_OAK_LED_COUNT - 1) {
    orange_oak_direction = -1;
  } else if (orange_oak_index == 0) {
    orange_oak_direction = 1;
  }
  orange_oak_index += orange_oak_direction;

  delay(750);
}