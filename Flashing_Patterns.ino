// void patterns(int pat)
// {

//   int totalColors = mode[m].numColors;
//   int currentColor = mode[m].currentColor;
//   int currentColor1 = mode[m].currentColor1;
//   int next = mode[m].nextColor;

//   switch (pat)
//   {

//   case 1:
//   { // strobe
//     if (on)
//     {
//       // set_led(ledh, leds, ledv);
//       setColor(hsv2rgb(1.0, 1.0, 1.0, col[3]));
//       duration = strobe_length;
//     }
//     if (!on)
//     {
//       clear_led();
//       setColor(hsv2rgb(0, 0, 0, col[3]));
//       duration = gap_length;
//     }
//     if (mainClock - prevTime > duration)
//     {
//       on = !on;
//       prevTime = mainClock;
//       // ledh = colors[0][0 + num];
//       // leds = colors[0][1 + num];
//       // ledv = colors[0][2 + num];
//       num += 3;
//       if (num >= 27)
//       {
//         num = 0;
//       }
//     }
//   }
//   }
// }

void strobe(unsigned char strobe_length, unsigned char gap_length, unsigned char group_gap_length)
{
  if (strobe_num == 0)
  {
    setHSVColor(ledh, leds, ledv);
    duration = strobe_length;
  }
  if (strobe_num == 1)
  {
    clear_led();
    duration = gap_length;
  }
  if (strobe_num == 2)
  {
    clear_led();
    duration = group_gap_length;
  }
  if (mainClock - prevTime > duration)
  {
    strobe_num++;
    // on = !on;
    prevTime = mainClock;
    ledh = modes[current_mode][0 + current_color];
    leds = modes[current_mode][1 + current_color];
    ledv = modes[current_mode][2 + current_color];
    current_color += 3;
    if (current_color >= 9)
    {
      current_color = 0;
    }
    if (group_gap_length == 0)
    {
      if (strobe_num > 1)
      {
        strobe_num = 0;
      }
    }
    else
    {
      if (strobe_num > 2)
      {
        strobe_num = 0;
      }
    }
  }
}
// void strobe(unsigned char strobe_length, unsigned char gap_length)
// {
//   if (on)
//   {
//     // set_led(ledh, leds, ledv);
//     // setColor(hsv2rgb(1.0, 1.0, 1.0, col));
//     setHSVColor(ledh, leds, ledv);
//     duration = strobe_length;
//   }
//   if (!on)
//   {
//     clear_led();
//     // setColor(hsv2rgb(0, 0, 0, col));
//     // setHSVColor(255, 255, 255);
//     duration = gap_length;
//   }
//   if (mainClock - prevTime > duration)
//   {
//     on = !on;
//     prevTime = mainClock;
//     ledh = modes[current_mode][0 + current_color];
//     leds = modes[current_mode][1 + current_color];
//     ledv = modes[current_mode][2 + current_color];
//     // Serial.print("Hue ");
//     // Serial.print(ledh);
//     // Serial.print(" ");
//     // Serial.print("Sat ");
//     // Serial.print(leds);
//     // Serial.print(" ");
//     // Serial.print("Value ");
//     // Serial.println(ledv);
//     // Serial.print("current_color ");
//     // Serial.println(current_color);
//     current_color += 3;
//     if (current_color >= 9)
//     {
//       current_color = 0;
//     }
//   }
// }

// void strobe(unsigned char mainClock, unsigned char strobe_length, unsigned char gap_length)
// {
//   if (on)
//   {
//     set_led(ledh, leds, ledv);
//     duration = strobe_length;
//   }
//   if (!on)
//   {
//     clear_led();
//     duration = gap_length;
//   }
//   if (mainClock - prevTime > duration)
//   {
//     on = !on;
//     prevTime = mainClock;
//     ledh = colors[0][0 + num];
//     leds = colors[0][1 + num];
//     ledv = colors[0][2 + num];
//     num += 3;
//     if (num >= 27)
//     {
//       num = 0;
//     }
//   }
// }
