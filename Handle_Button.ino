void handle_button()
{
  bool pressed = digitalRead(PIN_BUTTON) == HIGH; // Button is pressed when pin is low
  bool changed = pressed != was_pressed;          // If pressed state has changed, we might need to act

  Serial.print("State ");
  Serial.println(state);
  determine_loop(pressed);
  if (state == "mode_select")
  {
    mode_select(pressed, changed);
  }
  else if (state == "color_select")
  {
    color_select(pressed, changed);
  }
  else if (state == "pattern_select")
  {
    pattern_select(pressed, changed);
  }
  else if (state == "enter_sleep")
  {
    enter_sleep(pressed, changed);
  }
  since_press += 20;
  if (changed)
    since_press = 0;     // If state changed we need to reset since_press
  was_pressed = pressed; // Update was_pressed to this frame's button status
}

void mode_select(bool pressed, bool changed)
{

  if (pressed)
  {
    flash_menus();
  }
  else if (changed)
  {
    if (since_press < short_press && since_press != 0)
    {
      state = "mode_select";
      Serial.println("Next Mode");
      next_mode();
      since_press = 0;
      return;
    }
    else if (since_press < menu_2_length && since_press > menu_1_length)
    {
      Serial.println("enter_sleep");
      state = "enter_sleep";
      last_state = "mode_select";
      // EEPROM.write(0, 3);
    }
    else if (since_press < menu_3_length && since_press > menu_2_length)
    {
      Serial.println("color_select");
      state = "color_select";
      last_state = "mode_select";
    }
    else if (since_press < menu_4_length && since_press > menu_3_length)
    {
      Serial.println("pattern_select");
      state = "pattern_select";
    }
    else
    {
    }
  }
}

void color_select(bool pressed, bool changed)
{

  if (pressed)
  {
    flash_color_tint(current_color);
    // if (since_press == menu_1_length)
    // {
    //   // Serial.println(menu_1_length);
    //   flash(current_color, 255, 255);
    // }
  }
  else if (changed)
  {
    if (since_press < short_press && since_press != 0)
    {
      state = "color_select";
      Serial.println("Next Color");
      next_color();
      since_press = 0;
      return;
    }
    else if (since_press < menu_2_length && since_press > menu_1_length)
    {

      // fill_solid(leds, NUM_LEDS, CHSV(0, 0, 0));
      // FastLED.show();
      // state = "enter_sleep";
      // last_state = "mode_select";
      save_color();
      // EEPROM.write(0, 3);
    }
    else if (since_press < menu_3_length && since_press > menu_2_length)
    {

      set_tint();
      state = "color_select";
    }
    else
    {
    }
  }
}
void pattern_select(bool pressed, bool changed)
{

  if (pressed)
  {
    // flash_menus();
    // if (since_press == menu_1_length)
    // {
    //   // Serial.println(menu_1_length);
    //   flash(255, 0, 255);
    // }
  }
  else if (changed)
  {
    if (since_press < short_press && since_press != 0)
    {
      state = "pattern_select";
      Serial.println("Next Pattern");
      next_pattern();
      since_press = 0;
      return;
    }
    else if (since_press < menu_2_length && since_press > menu_1_length)
    {

      // fill_solid(leds, NUM_LEDS, CHSV(0, 0, 0));
      // FastLED.show();
      save_pattern();
      state = "mode_select";
      last_state = "pattern_select";
      // EEPROM.write(0, 3);
    }
    else if (since_press < menu_3_length && since_press > menu_2_length)
    {
      state = "setting_select";
      last_state = "mode_select";
    }
    else if (since_press < menu_4_length && since_press > menu_3_length)
    {
      state = "state_select";
    }
    else
    {
    }
  }
}

void enter_sleep(bool pressed, bool changed)
{
  clear_led();
  if (pressed)
  {
    if (since_press == menu_1_length)
    {
      // Serial.println(menu_1_length);
      flash(255, 0, 255);
    }
    else if (since_press == menu_5_length)
    {
      // Serial.println(menu_5_length);
      flash(255, 255, 255);
      flash(255, 100, 255);
    }
  }
  else if (changed)
  {
    if (since_press < menu_2_length && since_press > menu_1_length)
    {
      // Serial.println("Change States");
      state = last_state;
      // EEPROM.write(0, determine_state_number(last_state));
    }
    else if (since_press < menu_6_length && since_press > menu_5_length)
    {
      // Serial.println("Reset Settings");
      // reset_setting();
      state = "mode_select";
      // EEPROM.write(0, 0);
    }
    else
    {
    }
  }
}

// void handle_button()
// {
//   bool pressed = digitalRead(PIN_BUTTON) == HIGH; // Button is pressed when pin is low
//   bool changed = pressed != was_pressed;          // If pressed state has changed, we might need to act
//   since_press++;
//   if (state == "mode_select")
//   {
//     mode_select(pressed, changed);
//   }
//   if (changed)
//   {
//     since_press = 0; // If state changed we need to reset since_press
//   }
//   was_pressed = pressed; // Update was_pressed to this frame's button status
// }

// void mode_select(bool pressed, bool changed)
// {
//   mainClock = millis();
//   if (pressed)
//   {
//     flash(255, 255, 255);
//   }
//   else if (changed)
//   {
//     if (since_press < short_press && since_press != 0)
//     {
//       state = "mode_select";
//       // Serial.println("Next pattern");
//       next_mode();
//       since_press = 0;
//       return;
//     }
//     else
//     {
//       strobe(mainClock, 1, 30);
//     }
//   }
// }