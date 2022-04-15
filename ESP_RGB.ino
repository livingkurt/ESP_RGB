#include <Arduino.h>
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
// needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

//#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "ArduinoOTA.h" //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA

#include <Preferences.h>

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
  preferences.begin("ESP_RGB", false);
}

const uint8_t colors[9] = {
    255, 0, 0,
    0, 0, 255,
    0, 255, 0};

void strobe(unsigned char mainClock, unsigned char strobe_length, unsigned char gap_length)
{
  if (on)
  {
    set_led(ledr, ledg, ledb);
    duration = strobe_length;
  }
  if (!on)
  {
    clear_led();
    duration = gap_length;
  }
  if (mainClock - prevTime > duration)
  {
    on = !on;
    prevTime = mainClock;

    if (num > 3)
    {
      num = 0;
    }
    else
    {
      ledr = colors[0 + num];
      ledg = colors[1 + num];
      ledb = colors[2 + num];
      num++;
    }
  }
}

void set_led(unsigned char ledr, unsigned char ledg, unsigned char ledb)
{
  analogWrite(PIN_R, ledr);
  analogWrite(PIN_G, ledg);
  analogWrite(PIN_B, ledb);
}

void clear_led()
{
  analogWrite(PIN_R, 255);
  analogWrite(PIN_G, 255);
  analogWrite(PIN_B, 255);
}

void handle_button()
{
  bool pressed = digitalRead(PIN_BUTTON) == HIGH; // Button is pressed when pin is low
  bool changed = pressed != was_pressed;          // If pressed state has changed, we might need to act
  since_press++;
  if (state == "modes")
  {
    modes(pressed, changed);
  }
  if (changed)
  {
    since_press = 0; // If state changed we need to reset since_press
  }
  was_pressed = pressed; // Update was_pressed to this frame's button status
}

void modes(bool pressed, bool changed)
{
  mainClock = millis();
  if (pressed)
  {
    flash(0, 0, 0);
  }
  else
  {
    strobe(mainClock, 1, 30);
  }
}

void flash(unsigned char red, unsigned char green, unsigned char blue)
{
  set_led(red + 50, green + 50, blue + 50);
  delay(600);
  set_led(red + 150, green + 150, blue + 150);
  delay(300);
}

// void flash(unsigned char red, unsigned char green, unsigned char blue)
// {
//   set_led(red, green, blue);
//   delay(200);
//   set_led(255, 255, 255);
//   delay(200);
//   set_led(red, green, blue);
//   delay(200);
//   set_led(255, 255, 255);
//   delay(200);
//   set_led(red, green, blue);
//   delay(200);
//   set_led(255, 255, 255);
//   delay(200);
// }

void loop()
{
  ArduinoOTA.handle();
  handle_button();
}

// mode(mode_1_r,_1_4,_1_b, mode_2_r,mode_2_4,mode_2_b, mode_3_r,mode_3_4,mode_3_b, mode_4_r,mode_4_4,mode_4_b, mode_5_r,mode_5_4,mode_5_b, mode_6_r,mode_6_4,mode_6_b, mode_7_r,mode_7_4,mode_7_b, mode_8_r mode_8_4,mode_8_b,)

float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

float *hsv2rgb(float h, float s, float b, float *rgb)
{
  rgb[0] = b * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = b * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = b * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}

float *rgb2hsv(float r, float g, float b, float *hsv)
{
  float s = step(b, g);
  float px = mix(b, g, s);
  float py = mix(g, b, s);
  float pz = mix(-1.0, 0.0, s);
  float pw = mix(0.6666666, -0.3333333, s);
  s = step(px, r);
  float qx = mix(px, r, s);
  float qz = mix(pw, pz, s);
  float qw = mix(r, px, s);
  float d = qx - min(qw, py);
  hsv[0] = abs(qz + (qw - py) / (6.0 * d + 1e-10));
  hsv[1] = d / (qx + 1e-10);
  hsv[2] = qx;
  return hsv;
}