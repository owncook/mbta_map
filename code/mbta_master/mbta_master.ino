/* 
 * +---------------------+------------------------+------+------------+-------------+
 * | Line/Branch         | Description            | Pins | LED Count  | Total Stops |
 * +---------------------+------------------------+------+------------+-------------+
 * | Green Line (B)      | BC/BU                  | 2    | 18         | 70          |
 * | Green Line (C/E)    | Cleveland Cir/Boylston | 15   | 24         |             |
 * | Green Line (D)      | Riverside              | 19   | 13         |             |
 * | Green Line (E)      | Heath St               | 18   | 10         |             |
 * | Green Line (E2)     | Medford/Tufts          | 16   | 15         |             |
 * +---------------------+------------------------+------+------------+-------------+
 * | Red Line (Alewife)  | Alewife                | 4    | 20         | 31          |
 * | Red Line (Braintree)| Braintree              | 25   | 22         |             |
 * | Red Line (Mattapan) | Mattapan               | 23   | 12         |             |
 * +---------------------+------------------------+------+------------+-------------+
 * | Orange Line (Forest)| Forest Hills           | 22   | 23         | 20          |
 * | Orange Line (Oak)   | Oak Grove              | 27   | 14         |             |
 * +---------------------+------------------------+------+------------+-------------+
 * | Blue Line           | Entire Line            | 26   | 21         | 12          |
 * +---------------------+------------------------+------+------------+-------------+
 * | Commuter Rail       | Entire Line            | 14   | 25         | 14          |
 * +---------------------+------------------------+------+------------+-------------+
 * 
 * Available pins (grouped by level shifter): 
 * 2, 4, 15, 16
 * 14, 25, 26, 27
 * 18, 19, 22, 23
 *
 * 217 total LEDs   
 *
 * Logic Notes:                                                  
 * If stopped, light up the stop; if leaving, light up the next one; if approaching, 
 * light up the one before the stop.
 *
*/

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <vector>
#include <Wifi.h>
#include <keys.h>


/*
 * Pins, LED Counts, and Brightness
*/

// Low brightness to limit power consumtion
#define BRIGHTNESS 20

// Green line
#define GREENB_PIN 2
#define GREENB_LED_COUNT 18
#define GREENCE_PIN 15
#define GREENCE_LED_COUNT 24
#define GREEND_PIN 19
#define GREEND_LED_COUNT 13
#define GREENE_PIN 18
#define GREENE_LED_COUNT 10
#define GREENE2_PIN 16
#define GREENE2_LED_COUNT 15
// Red Line
#define RED_ALE_PIN 4
#define RED_ALE_LED_COUNT 20
#define RED_BRAIN_PIN 25
#define RED_BRAIN_LED_COUNT 22
#define RED_MATAP_PIN 23
#define RED_MATAP_LED_COUNT 12
// Orange Line
#define ORNG_FRST_PIN 22
#define ORNG_FRST_LED_COUNT 23
#define ORNG_OAK_PIN 27
#define ORNG_OAK_LED_COUNT 14
// Blue Line
#define BLUE_PIN 26
#define BLUE_LED_COUNT 21
// Commuter Rail
#define COMMUTER_PIN 14
#define COMMUTER_LED_COUNT 25

/*
 * Strip Definitions
*/
 
// Green Line
Adafruit_NeoPixel GreenB_strip          = Adafruit_NeoPixel(GREENB_LED_COUNT    , GREENB_PIN    , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel GreenCandE_strip      = Adafruit_NeoPixel(GREENCE_LED_COUNT   , GREENCE_PIN   , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel GreenD_strip          = Adafruit_NeoPixel(GREEND_LED_COUNT    , GREEND_PIN    , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel GreenECleveland_strip = Adafruit_NeoPixel(GREENE_LED_COUNT    , GREENE_PIN    , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel GreenEMedford_strip   = Adafruit_NeoPixel(GREENE2_LED_COUNT   , GREENE2_PIN   , NEO_GRB + NEO_KHZ800);
// Red Line
Adafruit_NeoPixel RedAlewife_strip      = Adafruit_NeoPixel(RED_ALE_LED_COUNT   , RED_ALE_PIN   , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel RedBraintree_strip    = Adafruit_NeoPixel(RED_BRAIN_LED_COUNT , RED_BRAIN_PIN , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel RedMatapan_strip      = Adafruit_NeoPixel(RED_MATAP_LED_COUNT , RED_MATAP_PIN , NEO_GRB + NEO_KHZ800);
// Orange Line
Adafruit_NeoPixel OrangeForest_strip    = Adafruit_NeoPixel(ORNG_FRST_LED_COUNT , ORNG_FRST_PIN , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel OrangeOakGrove_strip  = Adafruit_NeoPixel(ORNG_OAK_LED_COUNT  , ORNG_OAK_PIN  , NEO_GRB + NEO_KHZ800);
// Blue Line
Adafruit_NeoPixel BlueLine_strip        = Adafruit_NeoPixel(BLUE_LED_COUNT      , BLUE_PIN      , NEO_GRB + NEO_KHZ800);
// Commuter Rail
Adafruit_NeoPixel Commuter_strip        = Adafruit_NeoPixel(COMMUTER_LED_COUNT  , COMMUTER_PIN  , NEO_GRB + NEO_KHZ800);


/*
 * Fuctions/Global Variables
*/

// Structs for stops and corresponding LED
struct stop_LED {
  String stop_name;       // Name of stop
  int led_index;          // Index of LED on strip
  int strip_pin;          // Pin of the strip its on
  bool do_arrive_leave;   // Whether a light turns on for when a train is arriving/leaving
};

Stop_to_LED stop_LED = {
	// Red Line Alewife
	{"Alewife", 19, RED_ALE_PIN, false},
	{"Davis", 16, RED_ALE_PIN, true},
	{"Porter", 14, RED_ALE_PIN, true},
	{"Harvard", 12, RED_ALE_PIN, true},
	{"Central", 9, RED_ALE_PIN, true},
	{"Kendall/MIT", 6, RED_ALE_PIN, true},
	{"Charles/MGH", 4, RED_ALE_PIN, true},
	{"Park Street", 2, RED_ALE_PIN, true},

  // Red line Braintree
	{"South Station", 1, RED_BRAIN_PIN, true},
	{"Broadway", 4, RED_BRAIN_PIN, true},
	{"Andrew", 6, RED_BRAIN_PIN, true},
	{"JFK/UMass", 8, RED_BRAIN_PIN, true},
  {"North Quincy", 15, RED_BRAIN_PIN, true},
	{"Wollaston", 17, RED_BRAIN_PIN, true},
	{"Quincy Center", 19, RED_BRAIN_PIN, true},
	{"Quincy Adams", 20, RED_BRAIN_PIN, true},
	{"Braintree", 21, RED_BRAIN_PIN, false},
  
  // Red Line Mattapan
	{"Savin Hill", 1, RED_MATAP_PIN, false},
	{"Fields Corner", 2, RED_MATAP_PIN, false},
	{"Shawmut", 3, RED_MATAP_PIN, false},
	{"Ashmont", 4, RED_MATAP_PIN, false},
	{"Cedar Grove", 5, RED_MATAP_PIN, false},
	{"Butler", 6, RED_MATAP_PIN, false},
	{"Milton", 7, RED_MATAP_PIN, false},
	{"Central Avenue", 8, RED_MATAP_PIN, false},
	{"Valley Road", 9, RED_MATAP_PIN, false},
	{"Capen Street", 10, RED_MATAP_PIN, false},
	{"Mattapan", 11, RED_MATAP_PIN, false},
	
	// Orange Line oak
	{"Oak Grove", 13, ORNG_OAK_PIN, false},
	{"Malden Center", 12, ORNG_OAK_PIN, false},
	{"Wellington", 10, ORNG_OAK_PIN, false},
	{"Assembly", 9, ORNG_OAK_PIN, false},
	{"Sullivan Square", 8, ORNG_OAK_PIN, false},
	{"Community College", 7, ORNG_OAK_PIN, true},
	{"North Station", 3, ORNG_OAK_PIN, true},
	{"Haymarket", 1, ORNG_OAK_PIN, true},

  // Orange line forest
	{"Downtown Crossing", 0, ORNG_FRST_PIN, false},
	{"Chinatown", 4, ORNG_FRST_PIN, true},
	{"Tufts Medical Center", 7, ORNG_FRST_PIN, true},
	{"Back Bay", 9, ORNG_FRST_PIN, true},
	{"Massachusetts Avenue", 10, ORNG_FRST_PIN, false},
	{"Ruggles", 11, ORNG_FRST_PIN, false},
	{"Roxbury Crossing", 12, ORNG_FRST_PIN, false},
	{"Jackson Square", 14, ORNG_FRST_PIN, true},
	{"Stony Brook", 16, ORNG_FRST_PIN, true},
	{"Green Street", 19, ORNG_FRST_PIN, true},
	{"Forest Hills", 22, ORNG_FRST_PIN, false},

	// Green Line B line
  {"Boston College", 16, GREENB_PIN, false},
  {"Chestnut Hill Avenue", 14, GREENB_PIN, false},
  {"Babcock Street", 4, GREENB_PIN, false},
  {"Blandford Street", 0, GREENB_PIN, false},
  {"Packard's Corner", 6, GREENB_PIN, true},
  {"Boston University East", 1, GREENB_PIN, false},
  {"Boston University Central", 2, GREENB_PIN, false},
  {"Allston Street", 9, GREENB_PIN, false},
  {"Sutherland Road", 12, GREENB_PIN, false},
  {"Griggs Street", 8, GREENB_PIN, false},
  {"Warren Street", 10, GREENB_PIN, false},
  {"Washington Street", 11, GREENB_PIN, false},
  {"South Street", 15, GREENB_PIN, false},
  {"Harvard Avenue", 7, GREENB_PIN, false},
  {"Amory Street", 3, GREENB_PIN, false},
  {"Chiswick Road", 13, GREENB_PIN, false},

	// Green Line C line
  {"Cleveland Circle", 22, GREENCE_PIN, false},
  {"Englewood Avenue", 21, GREENCE_PIN, false},
  {"Saint Mary's Street", 10, GREENCE_PIN, false},
  {"Saint Paul Street", 13, GREENCE_PIN, false},
  {"Brandon Hall", 16, GREENCE_PIN, false},
  {"Summit Avenue", 15, GREENCE_PIN, false},
  {"Tappan Street", 19, GREENCE_PIN, false},
  {"Washington Square", 18, GREENCE_PIN, false},
  {"Kent Street", 12, GREENCE_PIN, false},
  {"Coolidge Corner", 14, GREENCE_PIN, false},
  {"Fairbanks Street", 17, GREENCE_PIN, false},
  {"Dean Road", 20, GREENCE_PIN, false},
  {"Arlington", 2, GREENCE_PIN, false},
  {"Hawes Street", 11, GREENCE_PIN, false},
  {"Kenmore", 7, GREENCE_PIN, true},
  {"Hynes Convention Center", 5, GREENCE_PIN, true},
  {"Boylston", 0, GREENCE_PIN, false},
  {"Copley", 3, GREENCE_PIN, false},

	// Green Line D line
	{"Brookline Hills", 3, GREEND_PIN, false},
	{"Chestnut Hill", 6, GREEND_PIN, false},
	{"Longwood", 1, GREEND_PIN, false},
	{"Fenway", 0, GREEND_PIN, false},
	{"Newton Highlands", 8, GREEND_PIN, false},
	{"Riverside", 12, GREEND_PIN, false},
	{"Waban", 10, GREEND_PIN, false},
	{"Eliot", 9, GREEND_PIN, false},
	{"Woodland", 11, GREEND_PIN, false},
	{"Beaconsfield", 4, GREEND_PIN, false},
	{"Reservoir", 5, GREEND_PIN, false},
	{"Newton Centre", 7, GREEND_PIN, false},
	{"Brookline Village", 2, GREEND_PIN, false},

	// Green Line E
	{"Brigham Circle", 5, GREENE_PIN, false},
	{"Prudential", 0, GREENE_PIN, false},
	{"Longwood Medical Area", 4, GREENE_PIN, false},
	{"Fenwood Road", 6, GREENE_PIN, false},
	{"Riverway", 8, GREENE_PIN, false},
	{"Back of the Hill", 8, GREENE_PIN, false},
	{"Heath Street", 9, GREENE_PIN, false},
	{"Northeastern University", 2, GREENE_PIN, false},
	{"Symphony", 1, GREENE_PIN, false},
	{"Mission Park", 7, GREENE_PIN, false},
	{"Museum of Fine Arts", 3, GREENE_PIN, false},

  // Green Line E2
	{"Medford/Tufts", 13, GREENE2_PIN, false},
	{"Ball Square", 12, GREENE2_PIN, false},
	{"Magoun Square", 10, GREENE2_PIN, false},
	{"Gilman Square", 9, GREENE2_PIN, false},
	{"East Somerville", 7, GREENE2_PIN, true},
	{"Lechmere", 4, GREENE2_PIN, false},
	{"Science Park/West End", 3, GREENE2_PIN, false},
	{"North Station", 1, GREENE2_PIN, true},
  {"Union Square", 14, GREENE2_PIN, false},
  
	// Blue Line
	{"Bowdoin", 0, BLUE_PIN, false},
	{"Government Center", 2, BLUE_PIN, true},
	{"State", 3, BLUE_PIN, true},
	{"Wood Island", 11, BLUE_PIN, false},
	{"Orient Heights", 13, BLUE_PIN, true},
	{"Airport", 10, BLUE_PIN, false},
	{"Maverick", 8, BLUE_PIN, true},
	{"Suffolk Downs", 15, BLUE_PIN, true},
	{"Aquarium", 5, BLUE_PIN, true},
	{"Beachmont", 17, BLUE_PIN, true},
	{"Revere Beach", 19, BLUE_PIN, false},
	{"Wonderland", 20, BLUE_PIN, false},

	// Haverhill Line 
	{"Andover", 18, COMMUTER_PIN, true},
	{"Ballardvale", 15, COMMUTER_PIN, true},
	{"Bradford", 22, COMMUTER_PIN, true},
	{"Reading", 10, COMMUTER_PIN, true},
	{"Wyoming Hill", 0, COMMUTER_PIN, false},
	{"Wakefield", 8, COMMUTER_PIN, true},
	{"Melrose/Cedar Park", 2, COMMUTER_PIN, true},
	{"North Wilmington", 13, COMMUTER_PIN, true},
	{"Oak Grove", 12, ORNG_OAK_PIN, true},
	{"Melrose Highlands", 4, COMMUTER_PIN, true},
	{"Greenwood", 6, COMMUTER_PIN, true},
	{"Lawrence", 20, COMMUTER_PIN, true},
};

// Wifi stuff
const char* WIFI_NAME = "MollyDK";
const char* WIFI_PASSWORD = "9782585527";

// Train lines
String train_lines[9] = {"Green-B", "Green-C", "Green-D", "Green-E", "Red", "Blue", "Orange", "CR-Haverhill", "Mattapan"};

// Query function


// Map function

// Light function


/*
  *   █████    ███    ██████    ██████  
  *  ██      ██   ██   ██   ██  ██      
  *  ██      ██   ██   ██   ██  █████   
  *  ██      ██   ██   ██   ██  ██      
  *   █████    ███    ██████    ██████  
*/

void setup() {

  Serial.begin(115200);

  // Connect to wifi
  Wifi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (Wifi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wifi...");
  }
  Serial.println("Connect to Wifi");

  /*
   * Initialize Strips 
  */

  // Green Line 
  GreenB_strip.begin();
  GreenCandE_strip.begin();
  GreenD_strip.begin();
  GreenECleveland_strip.begin();
  GreenEMedford_strip.begin();

  // Red Line 
  RedAlewife_strip.begin();
  RedBraintree_strip.begin();
  RedMatapan_strip.begin();

  // Orange Line 
  OrangeForest_strip.begin();
  OrangeOakGrove_strip.begin();

  // Blue Line 
  BlueLine_strip.begin();

  // Commuter Rail 
  Commuter_strip.begin();

  /*
   * Set brightness
  */

  GreenB_strip.setBrightness(BRIGHTNESS);
  GreenCandE_strip.setBrightness(BRIGHTNESS);
  GreenD_strip.setBrightness(BRIGHTNESS);
  GreenECleveland_strip.setBrightness(BRIGHTNESS);
  GreenEMedford_strip.setBrightness(BRIGHTNESS);

  RedAlewife_strip.setBrightness(BRIGHTNESS);
  RedBraintree_strip.setBrightness(BRIGHTNESS);
  RedMatapan_strip.setBrightness(BRIGHTNESS);

  OrangeForest_strip.setBrightness(BRIGHTNESS);
  OrangeOakGrove_strip.setBrightness(BRIGHTNESS);

  BlueLine_strip.setBrightness(BRIGHTNESS);

  Commuter_strip.setBrightness(BRIGHTNESS);
  
}


void loop() {

  // Query

  // Map

  // Light

}

// String request_link = "https://api-v3.mbta.com/vehicles?filter[route]=" + train_line + "&include=stop&api_key=" + API_KEY;

StaticJsonDocument<1024> query_mbta() {
   if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://api-v3.mbta.com/vehicles?include=stop&api_key=" + API_KEY;);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      StaticJsonDocument<1024> vechicle_data;

      
      // Deserialize directly from stream
      DeserializationError error = deserializeJson(vechicle_data, http.getStream());
      if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.f_str());
      } else {
        Serial.println("JSON parsed successfully:");
        return vechicle_data;
    } else {
      Serial.print("HTTP GET failed: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected");
  }

  return StaticJsonDocument<1024>();
}



struct stop_status {
  String stop;
  String status;
};
std::vector<stop_status> parse_mbta(StaticJsonDocument<1024> vehicle_data) {
  std::vector<stop_status> return_vec;
  if (vehicle_data.containsKey("data") && vehicle_data["data"].is<Array>()) {
    JsonArray vehicles = vehicle_data["data"].as<JsonArray>();


    for (JsonObject vehicle : vehicles) {
      String route_id = vehicle["relationships"]["route"]["data"]["id"];
      if (route_is_desired(route_id)) {
        stop_name = vehicle["relationships"]["stop"]["data"]["id"];
        stop_status = 
      }

    }

  }


}


bool route_is_desired(String& route) {
  for (int i = 0; i < 9; i++) {
    if (target == train_lines[i]) {
      return true;  // String found in the list
    }
  }
  return false;  // String not found in the list
}
















