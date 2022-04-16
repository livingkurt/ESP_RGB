#include <Arduino.h>
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
// needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

//#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "ArduinoOTA.h" //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA

#define PIN_R 13      // D7
#define PIN_G 12      // D6
#define PIN_B 14      // D5
#define PIN_BUTTON 15 // D8

unsigned char ledr = 255;
unsigned char ledg = 255;
unsigned char ledb = 255;
int num = 0;
uint32_t since_press = 0; // Tracks how long since last button press
bool was_pressed = false; // Tracks if the button was pressed in previous frame
bool on;
unsigned long mainClock, prevTime, duration, prevTime2, duration2, duration3, duration4;

char *state = "modes"; // Current state of the light
char *last_state = ""; // Current state of the light

float col[3];
float hue = 0.0;

// typedef void (*FlashingPatternsList[])();
//
// FlashingPatternsList flashing_patterns = {
//     strobe,
// };
// int num_modes = (sizeof(flashing_patterns) / sizeof(flashing_patterns[0]));

const uint8_t available_colors[48] = {
    0, 0, 0,       // Blank
    255, 255, 255, // White
    255, 0, 0,     // Red
    255, 128, 0,   // Orange
    255, 255, 0,   // Yellow
    0, 255, 0,     // Green
    0, 255, 128,   // Seafoam
    0, 255, 255,   // Cyan
    0, 128, 255,   // Light Blue
    0, 0, 255,     // Blue
    127, 0, 255,   // Purple
    255, 0, 255,   // Pink
    255, 0, 127,   // Magenta
    246, 93, 95,   // Coral
    251, 233, 198, // Cream
    251, 233, 198, // Cream

};

void setup()
{
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  prevTime = 0;
  duration = 0;
  Serial.begin(115200);
  Serial.println("Booting");
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP_RGB");
  Serial.println("connected...yeey :)");

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
  ArduinoOTA.begin();
  Serial.println("Ready OTA8");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// const char *states[4] = {
//     "modes",
//     "party_modes",
//     "colors",
//     "all_modes",
// };

// int num_states = (sizeof(states) / sizeof(states[0]));

// const int *state_colors[4] = {
//     0,
//     32,
//     64,
//     96};
// int num_state_colors = (sizeof(state_colors) / sizeof(state_colors[0]));

// const char *settings[5] = {
//     "autoplay",
//     "interval_length",
//     "random_order",
//     "random_interval",
//     "back_to_modes",
//     // "save_settings",
// };

// int num_settings = (sizeof(settings) / sizeof(settings[0]));

// const int *setting_colors[5] = {
//     110,
//     130,
//     150,
//     170,
//     190};

// int num_setting_colors = (sizeof(setting_colors) / sizeof(setting_colors[0]));

// const char *intervals[3] = {
//     "long",
//     "mid",
//     "short",
// };

// int num_intervals = (sizeof(intervals) / sizeof(intervals[0]));

// const int *interval_colors[3] = {
//     255,
//     170,
//     85};

const uint8_t colors[2][48] = {{
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                               },
                               {
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                                   0,
                                   0,
                                   255,
                               }};

void loop()
{
  ArduinoOTA.handle();
  handle_button();
}
