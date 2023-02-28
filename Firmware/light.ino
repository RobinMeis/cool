bool light_is_on = false;

void light_init() {
  ledcSetup(0, 5000, 8);
  ledcAttachPin(PIN_LIGHT, 0);
}

void light_on() {
  light_is_on = true;
  ledcWrite(0, preferences.getUChar("brightness", 254));
  light_status();
}

void light_off() {
  light_is_on = false;
  ledcWrite(0, 0);
  light_status();
}

void light_set_brightness(int value) {
  if (value > 0 && value <= 254) {
    preferences.putUChar("brightness", value);
    light_on();
  }
}

void light_status() {
  if (light_is_on) {
    client.publish(topic_status_light, "ON", true);
    char value[5];
    itoa(preferences.getUChar("brightness", 254), value, 10);
    client.publish(topic_status_light_brightness, value);
  } else {
    client.publish(topic_status_light, "OFF", true);
  }
}
