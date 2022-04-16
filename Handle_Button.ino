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