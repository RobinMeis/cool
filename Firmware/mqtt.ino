unsigned long mqtt_last_reconnect=0;

void mqtt_init() {
  pinMode(LED_MQTT, OUTPUT);
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

void mqtt_reconnect() { //Check if MQTT server is connected and reconnect if required
  if (client.connected()) {
    digitalWrite(LED_MQTT, HIGH);
  } else {
    digitalWrite(LED_MQTT, LOW);
    if (WiFi.status() == WL_CONNECTED && millis() - mqtt_last_reconnect > 5000) { //Reconnect only if wifi is connected
      Serial.println("Connecting to MQTT Server...");

      for (int i=0; i<5; ++i) {
        digitalWrite(LED_MQTT, HIGH);
        delay(100);
        digitalWrite(LED_MQTT, LOW);
        delay(100);
      }

      if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD, topic_availability, 1, true, "offline")) {
        client.setBufferSize(2048);
        client.subscribe(topic_control_light);
        client.subscribe(topic_control_light_brightness);
        client.subscribe(topic_control_preset_mode);
        client.subscribe(topic_control_preset_mode_timeout);
        client.subscribe(topic_control_compressor_cycle);
        client.subscribe(topic_control_reset_button);
        client.subscribe(topic_control_updater);

        mqtt_availability();
        homeassistant_autodiscovery();
        compressor_status();
        door_status();
        light_status();
        preset_mode_status();

        #if HAS_FRIDGE==true
          client.subscribe(topic_control_fridge_mode);
          client.subscribe(topic_control_fridge_temperature_low);
          client.subscribe(topic_control_fridge_temperature_high);
          fridge_status();
        #endif

        #if HAS_FREEZER==true
          client.subscribe(topic_control_freezer_mode);
          client.subscribe(topic_control_freezer_temperature_low);
          client.subscribe(topic_control_freezer_temperature_high);
          freezer_status();
        #endif
      }
      mqtt_last_reconnect = millis();
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, payload);

  payload[length] = '\0';

  if (strcmp(topic, topic_control_light) == 0) { //Light on/off
    if (strcmp((char*)payload, "ON") == 0) {
      light_on();
    } else if (strcmp((char*)payload, "OFF") == 0) {
      light_off();
    }
  } else if (strcmp(topic, topic_control_light_brightness) == 0) { //Light brightness
    int value = atoi((char*)payload);
    light_set_brightness(value);
  } else if (strcmp(topic, topic_control_preset_mode) == 0) { //Preset mode
    if (strcmp((char*)payload, "none") == 0) {
      preset_mode_none();
    } else if (strcmp((char*)payload, "eco") == 0) {
      preset_mode_eco();
    } else if (strcmp((char*)payload, "boost") == 0) {
      preset_mode_boost();
    }
  } else if (strcmp(topic, topic_control_preset_mode_timeout) == 0) { //Preset mode timeout duration
    preset_mode_set_timeout_duration(atoi((char*)payload));
  } else if (strcmp(topic, topic_control_compressor_cycle) == 0) { //Compressor minimum cycle duration
    compressor_set_minimum_cycle_duration(atoi((char*)payload));
  #if HAS_FRIDGE==true
  } else if (strcmp(topic, topic_control_fridge_mode) == 0) { //Fridge mode
    if (strcmp((char*)payload, "ON") == 0)
      fridge_turn_on();
    else if (strcmp((char*)payload, "OFF") == 0)
      fridge_turn_off();
  } else if (strcmp(topic, topic_control_fridge_temperature_low) == 0) { //Fridge minimum temperature
    fridge_set_minimum(atof((char*)payload));
  } else if (strcmp(topic, topic_control_fridge_temperature_high) == 0) { //Fridge minimum temperature
    fridge_set_maximum(atof((char*)payload));
  #endif
  #if HAS_FREEZER==true
  } else if (strcmp(topic, topic_control_freezer_mode) == 0) { //Fridge mode
    if (strcmp((char*)payload, "ON") == 0)
      freezer_turn_on();
    else if (strcmp((char*)payload, "OFF") == 0)
      freezer_turn_off();
  } else if (strcmp(topic, topic_control_freezer_temperature_low) == 0) { //Fridge minimum temperature
    freezer_set_minimum(atof((char*)payload));
  } else if (strcmp(topic, topic_control_freezer_temperature_high) == 0) { //Fridge minimum temperature
    freezer_set_maximum(atof((char*)payload));
  #endif
  } else if (strcmp(topic, topic_control_reset_button) == 0) { //Reset button
    reset();
  }
}

void mqtt_availability() {
  client.publish(topic_availability, "online", true);
}

void mqtt_loop() {
  mqtt_reconnect();
  client.loop();
}

void mqtt_generate_topic(char *topic_variable, char *topic) {
  sprintf(topic_variable, "%s/%s", host_name.c_str(), topic);
}
