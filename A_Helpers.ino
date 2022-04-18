

// void set_led(unsigned char ledh, unsigned char leds, unsigned char ledv)
// {
//   ledh = abs(ledh - 255);
//   leds = abs(leds - 255);
//   ledv = abs(ledv - 255);
//   analogWrite(PIN_R, ledh);
//   analogWrite(PIN_G, leds);
//   analogWrite(PIN_B, ledv);
// }

void setColor(float *rgb)
{
  analogWrite(PIN_R, (int)((1.0 - rgb[0]) * 255));
  analogWrite(PIN_G, (int)((1.0 - rgb[1]) * 255));
  analogWrite(PIN_B, (int)((1.0 - rgb[2]) * 255));
}

void setHSVColor(unsigned char h, unsigned char s, unsigned char v)
{
  float hue = h / 255.0;
  float saturation = s / 255.0;
  float value = v / 255.0;
  setColor(hsv2rgb(hue, saturation, value, col));
}

void clear_led()
{
  analogWrite(PIN_R, 255);
  analogWrite(PIN_G, 255);
  analogWrite(PIN_B, 255);
}

void flash(unsigned char h, unsigned char s, unsigned char v)
{
  // unsigned long time_now = 0;
  // if (millis() >= time_now + 600)
  // {
  //   time_now += 600;
  //   setHSVColor(h, s, v - 50);
  // }
  // if (millis() >= time_now + 300)
  // {
  //   time_now += 100;
  //   setHSVColor(h, s, v - 100);
  // }
  for (uint8_t i = 0; i < 5; i++)
  {
    for (uint8_t j = 0; j < 200; j++)
    {
      if (j < 100)
        setHSVColor(0, 0, 0);
      else
        setHSVColor(h, s, v);
    }
  }
  since_press += 1000;
}

void flash_menus()
{
  if (since_press == menu_1_length)
  {
    flash(255, 255, 255);
  }
  else if (since_press == menu_2_length)
  {
    flash(160, 255, 255);
  }
  else if (since_press == menu_3_length)
  {
    flash(64, 255, 255);
  }
}

void flash_color_tint(char current_color)
{
  if (since_press == menu_1_length)
  {
    flash(current_color, 255, 255);
  }
  else if (since_press == menu_2_length)
  {
    flash(current_color, 255, 170);
  }
  else if (since_press == menu_3_length)
  {
    flash(current_color, 255, 85);
  }
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

float fract(float x)
{
  return x - int(x);
}

float mix(float a, float b, float t)
{
  return a + (b - a) * t;
}

float step(float e, float x)
{
  return x < e ? 0.0 : 1.0;
}

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
