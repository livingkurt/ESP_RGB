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
//       // set_led(ledr, ledg, ledb);
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
//       // ledr = colors[0][0 + num];
//       // ledg = colors[0][1 + num];
//       // ledb = colors[0][2 + num];
//       num += 3;
//       if (num >= 27)
//       {
//         num = 0;
//       }
//     }
//   }
//   }
// }

void strobe(unsigned char mainClock, unsigned char strobe_length, unsigned char gap_length)
{
  if (on)
  {
    // set_led(ledr, ledg, ledb);
    setColor(hsv2rgb(1.0, 1.0, 1.0, col));
    duration = strobe_length;
  }
  if (!on)
  {
    clear_led();
    setColor(hsv2rgb(0, 0, 0, col));
    duration = gap_length;
  }
  if (mainClock - prevTime > duration)
  {
    on = !on;
    prevTime = mainClock;
    // ledr = colors[0][0 + num];
    // ledg = colors[0][1 + num];
    // ledb = colors[0][2 + num];
    num += 3;
    if (num >= 27)
    {
      num = 0;
    }
  }
}

// void strobe(unsigned char mainClock, unsigned char strobe_length, unsigned char gap_length)
// {
//   if (on)
//   {
//     set_led(ledr, ledg, ledb);
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
//     ledr = colors[0][0 + num];
//     ledg = colors[0][1 + num];
//     ledb = colors[0][2 + num];
//     num += 3;
//     if (num >= 27)
//     {
//       num = 0;
//     }
//   }
// }
