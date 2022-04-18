void determine_loop(long pressed)
{
  // if (!pressed)
  // {
  if (state == "mode_select")
  {
    handle_mode_change();
  }
  if (state == "color_select")
  {
    handle_color_change();
  }
  if (state == "pattern_select")
  {
    handle_pattern_change();
  }
  if (state == "enter_sleep")
  {
    handle_enter_sleep();
  }
  // }
}

void handle_mode_change()
{

  mode_select[current_mode];
  patterns(current_pattern);
}

void handle_color_change()
{

  colors[current_color];
  // strobe(mainClock, 500, 0);
}
void handle_pattern_change()
{

  patterns(current_pattern);
  // strobe(mainClock, 500, 0);
}

void handle_enter_sleep()
{
  clear_led();
  // FastLED.show();
}