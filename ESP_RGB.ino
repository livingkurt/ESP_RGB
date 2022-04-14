#include <Arduino.h>

int PIN_R = 13;
int PIN_G = 12;
int PIN_B = 14;
int ledr = 255;
int ledg = 255;
int ledb = 255;
int num = 0;

//uint32_t since_press = 0; // Tracks how long since last button press
//bool was_pressed = false; // Tracks if the button was pressed in previous frame


bool on;
unsigned long mainClock, prevTime, duration, prevTime2, duration2, duration3, duration4;


void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  prevTime = 0;
  duration = 0;
}


void loop() {
  mainClock = millis();
  strobe(mainClock, 1, 30);
}

const uint8_t colors[9] = {
  255, 0, 0,
  0, 0, 255,
  0, 255, 0
};

void strobe(int mainClock, int strobe_length, int gap_length) {
  if (on) {
    set_led(ledr, ledg, ledb);
    duration = strobe_length;
  }
  if (!on) {
    clear_led();
    duration = gap_length;

  }
  if (mainClock - prevTime > duration) {
    on = !on;
    prevTime = mainClock;
    
    if (num > 3) {
      num = 0;
    }
    else {
      ledr = colors[0 + num];
      ledg = colors[1 + num];
      ledb = colors[2 + num];
      num++;
    }
    
  }
}


void set_led(int ledr, int ledg, int ledb)
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
