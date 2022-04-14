#include <Arduino.h>

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

void setup()
{
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  prevTime = 0;
  duration = 0;
}

void loop()
{
  handle_button();
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