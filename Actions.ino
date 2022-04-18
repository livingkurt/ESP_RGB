void next_mode()
{
  current_mode = (current_mode + 1) % ARRAY_SIZE(modes);
  Serial.println(current_mode);
}

void next_color()
{
  current_color = (current_color + 1) % ARRAY_SIZE(colors);
  Serial.println(current_color);
}

void next_pattern()
{
  current_pattern = (current_pattern + 1) % 7;
  Serial.println(current_pattern);
}

void save_color()
{
  // current_color = (current_color + 1) % ARRAY_SIZE(colors);
  // Serial.println(current_color);
}

void save_pattern()
{
  // current_color = (current_color + 1) % ARRAY_SIZE(colors);
  // Serial.println(current_color);
}

void set_tint()
{
  // current_color = (current_color + 1) % ARRAY_SIZE(colors);
  // Serial.println(current_color);
}
