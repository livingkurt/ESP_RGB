

// void set_led(unsigned char ledr, unsigned char ledg, unsigned char ledb)
// {
//   ledr = abs(ledr - 255);
//   ledg = abs(ledg - 255);
//   ledb = abs(ledb - 255);
//   analogWrite(PIN_R, ledr);
//   analogWrite(PIN_G, ledg);
//   analogWrite(PIN_B, ledb);
// }

void setColor(float *rgb)
{
  analogWrite(PIN_R, (int)((1.0 - rgb[0]) * 255));
  analogWrite(PIN_G, (int)((1.0 - rgb[1]) * 255));
  analogWrite(PIN_B, (int)((1.0 - rgb[2]) * 255));
}

void clear_led()
{
  analogWrite(PIN_R, 255);
  analogWrite(PIN_G, 255);
  analogWrite(PIN_B, 255);
}

void flash(unsigned char red, unsigned char green, unsigned char blue)
{
  // set_led(red + 50, green + 50, blue + 50);
  setColor(hsv2rgb(1.0, 1.0, 1.0, col));
  delay(600);
  // set_led(red + 150, green + 150, blue + 150);
  setColor(hsv2rgb(1.0, 1.0, 1.0, col));
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
