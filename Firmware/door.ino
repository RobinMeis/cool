bool door_is_open;
unsigned long door_debounce_start=0;

void door_init() {
  pinMode(PIN_DOOR, INPUT_PULLUP);
  if (digitalRead(PIN_DOOR) == !PIN_DOOR_ACTIVE_LOW)
    door_open();
  else
    door_closed();
}

void door_open() {
  if (door_is_open || millis() - door_debounce_start > DOOR_DEBOUNCE_TIME)
    return;

  door_debounce_start = millis();
  door_is_open = true;
  light_on();
  door_status();
}

void door_closed() {
  if (!door_is_open || millis() - door_debounce_start > DOOR_DEBOUNCE_TIME)
    return;

  door_debounce_start = millis();
  door_is_open = false;
  light_off();
  door_status();
}

void door_status() {
  if (door_is_open)
    client.publish(topic_status_door, "open");
  else
    client.publish(topic_status_door, "closed");
}

void door_loop() {
  if (digitalRead(PIN_DOOR) == !PIN_DOOR_ACTIVE_LOW)
    door_open();
  else
    door_closed();
}
