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

#define NUM_MODES 2
#define NUM_COLORS 45
#define NUM_MODE_COLORS 8
#define NUM_PATTERNS 10

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

unsigned char ledh = 255;
unsigned char leds = 255;
unsigned char ledv = 255;
// int num = 0;
uint32_t since_press = 0; // Tracks how long since last button press
bool was_pressed = false; // Tracks if the button was pressed in previous frame
bool on;
unsigned char strobe_num = 0;
unsigned long mainClock, prevTime, duration, prevTime2, duration2, duration3, duration4;

char *state = "mode_select";
char *last_state = "";
char current_mode = 0;
char current_color = 0;
char current_pattern = 1;

float col[3];

int short_press = 1000;
int menu_1_length = 2000;
int menu_2_length = 3000;
int menu_3_length = 4000;
int menu_4_length = 5000;
int menu_5_length = 6000;
int menu_6_length = 7000;

// typedef void (*FlashingPatternsList[])();
//
// FlashingPatternsList flashing_patterns = {
//     strobe,
// };
// int num_mode_select = (sizeof(flashing_patterns) / sizeof(flashing_patterns[0]));

// const uint8_t available_colors[48] = {
//     255, 255, 255, // White
//     0, 0, 0,       // Blank
//     255, 0, 0,     // Red
//     255, 128, 0,   // Orange
//     255, 255, 0,   // Yellow
//     0, 255, 0,     // Green
//     0, 255, 128,   // Seafoam
//     0, 255, 255,   // Cyan
//     0, 128, 255,   // Light Blue
//     0, 0, 255,     // Blue
//     127, 0, 255,   // Purple
//     255, 0, 255,   // Pink
//     255, 0, 127,   // Magenta
//     246, 93, 95,   // Coral
//     251, 233, 198, // Cream
//     251, 233, 198, // Cream

// };

void patterns(int pat)
{
  Serial.print("patterns ");
  Serial.println(pat);
  switch (pat)
  {

  case 1:
  { // Strobe
    return strobe(5, 8, 0);
  }
  case 2:
  { // Strobie
    return strobe(3, 22, 0);
  }
  case 3:
  { // Dops
    return strobe(2, 13, 0);
  }
  case 4:
  { // Hyoer
    return strobe(25, 25, 0);
  }
  case 5:
  { // Ribbon
    return strobe(12, 0, 0);
  }
  case 6:
  { // Blaster
    return strobe(3, 0, 70);
  }
  case 7:
  { // Auto Blaster
    return strobe(3, 5, 70);
  }
  }
}
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
//     "mode_select",
//     "party_mode_select",
//     "colors",
//     "all_mode_select",
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
//     "back_to_mode_select",
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

const uint8_t colors[NUM_COLORS] = {
    255, 255, 0,   // Blank
    255, 0, 255,   // White
    0, 255, 255,   // Red
    32, 255, 255,  // Orange
    64, 255, 255,  // Yellow
    96, 255, 255,  // Green
    106, 255, 255, // Seafoam
    128, 255, 255, // Cyan
    149, 255, 255, // Light Blue
    160, 255, 255, // Blue
    192, 255, 255, // Purple
    224, 255, 255, // Pink
    234, 255, 255, // Magenta
    11, 232, 163,  // Coral
    142, 54, 250,  // Cream

};

int num_colors = (sizeof(colors) / sizeof(colors[0]));

const uint8_t modes[6][9] = {{
                                 128, 255, 255, // Cyan
                                 11, 232, 163,  // Coral
                                 57, 54, 250,   // Cream

                             },
                             {
                                 160, 0, 255,   // White
                                 40, 255, 255,  // Red
                                 200, 255, 255, // Green

                             },
                             {
                                 220, 0, 255,   // White
                                 0, 255, 255,   // Red
                                 140, 255, 255, // Green

                             },
                             {
                                 100, 0, 255,  // White
                                 0, 255, 255,  // Red
                                 50, 255, 255, // Green

                             },
                             {
                                 60, 0, 255,    // White
                                 255, 255, 255, // Red
                                 80, 255, 255,  // Green

                             },
                             {
                                 255, 0, 255,  // White
                                 0, 255, 255,  // Red
                                 96, 255, 255, // Green

                             }};

int num_modes = (sizeof(modes) / sizeof(modes[0]));
// int num_modes = (sizeof(modes[0]) / sizeof(modes[0][0]));

void loop()
{
  mainClock = millis();
  ArduinoOTA.handle();
  handle_button();
}
