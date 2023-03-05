bool door_is_open;
unsigned long door_debounce_start=0;

void door_init() {
  pinMode(PIN_DOOR, INPUT);
  door_read(true);
}

void door_read(bool force_update) {
  if (digitalRead(PIN_DOOR) == !PIN_DOOR_ACTIVE_LOW)
    door_open(force_update);
  else
    door_closed(force_update);
}

void door_open(bool force_update) {
  if (!force_update && (door_is_open || millis() - door_debounce_start < DOOR_DEBOUNCE_TIME))
    return;
  
  door_debounce_start = millis();
  door_is_open = true;
  light_on();
  door_status();
}

void door_closed(bool force_update) {
  if (!force_update && (!door_is_open || millis() - door_debounce_start < DOOR_DEBOUNCE_TIME))
    return;

  door_debounce_start = millis();
  door_is_open = false;
  light_off();
  door_status();
}

void door_status() {
  if (door_is_open)
    client.publish(topic_status_door, "open", true);
  else
    client.publish(topic_status_door, "closed", true);
}

void door_loop() {
  door_read(false);
}
