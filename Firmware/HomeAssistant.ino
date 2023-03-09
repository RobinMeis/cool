void homeassistant_availability() {
  client.publish(topic_availability, "online");
}

void homeassistant_autodiscovery_device(JsonDocument &doc) {
  JsonObject device = doc.createNestedObject("device");
  JsonArray identifiers = device.createNestedArray("identifiers");
  identifiers.add(host_name);
  device["manufacturer"] = "Robin Meis";
  device["model"] = "Cool";
  device["name"] = DEVICE_NAME;
  device["sw_version"] = SW_VERSION;
  device["suggested_area"] = "Kitchen";
}

void homeassistant_autodiscovery_hvac_fridge() {
  DynamicJsonDocument doc(2048);
  homeassistant_autodiscovery_device(doc);
  doc["name"] = "Fridge";
  doc["unique_id"] = host_name + "_fridge";
  doc["object_id"] = doc["unique_id"];
  doc["availability_topic"] = topic_availability;
  doc["icon"] = "mdi:thermometer";
  doc["min_temp"] = FRIDGE_HVAC_MINIMUM;
  doc["max_temp"] = FRIDGE_HVAC_MAXIMUM;
  doc["current_temperature_topic"] = topic_status_temperature;
  doc["current_temperature_template"] = "{{ value_json.fridge | round(2) }}";
  doc["temperature_unit"] = "C";
  doc["temp_step"] = 0.1;
  doc["mode_command_topic"] = topic_control_fridge_mode;
  doc["mode_command_template"] = "{% if value=='cool' %} ON {% else %} OFF {% endif %}";
  doc["mode_state_topic"] = topic_status_fridge;
  doc["mode_state_template"] = "{% if value_json.on==true %} cool {% else %} off {% endif %}";
  doc["preset_mode_command_topic"] = topic_control_preset_mode;
  doc["preset_mode_state_topic"] = topic_status_preset_mode;
  doc["preset_mode_value_template"] = "{{ value_json.mode }}";
  doc["temperature_low_command_topic"] = topic_control_fridge_temperature_low;
  doc["temperature_high_command_topic"] = topic_control_fridge_temperature_high;
  doc["temperature_low_state_topic"] = topic_status_fridge;
  doc["temperature_high_state_topic"] = topic_status_fridge;
  doc["temperature_low_state_template"] = "{{ value_json.temperature.min | round(2) }}";
  doc["temperature_high_state_template"] = "{{ value_json.temperature.max | round(2) }}";

  JsonArray modes = doc.createNestedArray("modes");
  modes.add("cool");
  modes.add("off");

  JsonArray preset_modes = doc.createNestedArray("preset_modes");
  preset_modes.add("boost");
  preset_modes.add("eco");

  char topic[60];
  homeassistant_generate_topic(topic, "climate", "fridge");

  serializeJson(doc, buffer);
  client.publish(topic, buffer, true);
}

void homeassistant_autodiscovery_hvac_freezer() {
  DynamicJsonDocument doc(2048);
  homeassistant_autodiscovery_device(doc);
  doc["name"] = "Freezer";
  doc["unique_id"] = host_name + "_freezer";
  doc["object_id"] = doc["unique_id"];
  doc["availability_topic"] = topic_availability;
  doc["icon"] = "mdi:snowflake-thermometer";
  doc["min_temp"] = FREEZER_HVAC_MINIMUM;
  doc["max_temp"] = FREEZER_HVAC_MAXIMUM;
  doc["current_temperature_topic"] = topic_status_temperature;
  doc["current_temperature_template"] = "{{ value_json.freezer | round(2) }}";
  doc["temperature_unit"] = "C";
  doc["temp_step"] = 0.1;
  doc["mode_command_topic"] = topic_control_freezer_mode;
  doc["mode_command_template"] = "{% if value=='cool' %} ON {% else %} OFF {% endif %}";
  doc["mode_state_topic"] = topic_status_freezer;
  doc["mode_state_template"] = "{% if value_json.on==true %} cool {% else %} off {% endif %}";
  doc["preset_mode_command_topic"] = topic_control_preset_mode;
  doc["preset_mode_value_template"] = "{{ value_json.mode }}";
  doc["preset_mode_state_topic"] = topic_status_preset_mode;
  doc["temperature_low_command_topic"] = topic_control_freezer_temperature_low;
  doc["temperature_high_command_topic"] = topic_control_freezer_temperature_high;
  doc["temperature_low_state_topic"] = topic_status_freezer;
  doc["temperature_high_state_topic"] = topic_status_freezer;
  doc["temperature_low_state_template"] = "{{ value_json.temperature.min | round(2) }}";
  doc["temperature_high_state_template"] = "{{ value_json.temperature.max | round(2) }}";

  JsonArray modes = doc.createNestedArray("modes");
  modes.add("cool");
  modes.add("off");

  JsonArray preset_modes = doc.createNestedArray("preset_modes");
  preset_modes.add("boost");
  preset_modes.add("eco");

  char topic[60];
  homeassistant_generate_topic(topic, "climate", "freezer");

  serializeJson(doc, buffer);
  client.publish(topic, buffer, true);
}

void homeassistant_autodiscovery_door() {
  DynamicJsonDocument doc(1024);
  homeassistant_autodiscovery_device(doc);
  doc["name"] = "Door";
  doc["unique_id"] = host_name + "_door";
  doc["object_id"] = doc["unique_id"];
  doc["state_topic"] = topic_status_door;
  doc["device_class"] = "door";
  doc["payload_on"] = "open";
  doc["payload_off"] = "closed";
  doc["availability_topic"] = topic_availability;

  char topic[60];
  homeassistant_generate_topic(topic, "binary_sensor", "door");
  
  serializeJson(doc, buffer);
  client.publish(topic, buffer, true);
}

void homeassistant_autodiscovery_light() {
  DynamicJsonDocument doc(1024);
  homeassistant_autodiscovery_device(doc);
  doc["name"] = "Light";
  doc["unique_id"] = host_name + "_light";
  doc["object_id"] = doc["unique_id"];
  doc["command_topic"] = topic_control_light;
  doc["state_topic"] = topic_status_light;
  doc["brightness"] = true;
  doc["brightness_scale"] = 255;
  doc["brightness_command_topic"] = topic_control_light_brightness;
  doc["brightness_state_topic"] = topic_status_light_brightness;
  doc["availability_topic"] = topic_availability;
  doc["entity_category"] = "config";

  char topic[60];
  homeassistant_generate_topic(topic, "light", "light");
  
  serializeJson(doc, buffer);
  client.publish(topic, buffer, true);
}

void homeassistant_autodiscovery_compressor() {
  DynamicJsonDocument doc(1024);
  homeassistant_autodiscovery_device(doc);
  doc["name"] = "Compressor";
  doc["unique_id"] = host_name + "_compressor";
  doc["object_id"] = doc["unique_id"];
  doc["state_topic"] = topic_status_compressor;
  doc["device_class"] = "running";
  doc["value_template"] = "{{ value_json.on }}";
  doc["payload_on"] = true;
  doc["payload_off"] = false;
  doc["availability_topic"] = topic_availability;

  char topic[65];
  homeassistant_generate_topic(topic, "binary_sensor", "compressor");
  
  serializeJson(doc, buffer);
  client.publish(topic, buffer, true);
}

void homeassistant_autodiscovery_compressor_minimum_cycle_duration() {
  DynamicJsonDocument doc(1024);
  homeassistant_autodiscovery_device(doc);
  doc["name"] = "Compressor minimum cycle duration";
  doc["icon"] = "mdi:timer-outline";
  doc["unique_id"] = host_name + "_compressor_minimum_cycle_duration";
  doc["object_id"] = doc["unique_id"];
  doc["state_topic"] = topic_status_compressor;
  doc["command_topic"] = topic_control_compressor_cycle;
  doc["value_template"] = "{{ value_json.minimum_cycle_duration }}";
  doc["min"] = 0;
  doc["max"] = 30;
  doc["unit_of_measurement"] = "minutes";
  doc["availability_topic"] = topic_availability;
  doc["entity_category"] = "config";

  char topic[85];
  homeassistant_generate_topic(topic, "number", "compressor_minimum_cycle_duration");
  
  serializeJson(doc, buffer);
  client.publish(topic, buffer, true);
}

void homeassistant_autodiscovery_preset_timeout() {
  DynamicJsonDocument doc(1024);
  homeassistant_autodiscovery_device(doc);
  doc["name"] = "Preset timeout";
  doc["icon"] = "mdi:timer-outline";
  doc["unique_id"] = host_name + "_preset_timeout";
  doc["object_id"] = doc["unique_id"];
  doc["state_topic"] = topic_status_preset_mode;
  doc["command_topic"] = topic_control_preset_mode_timeout;
  doc["value_template"] = "{{ value_json.timeout_duration }}";
  doc["min"] = 10;
  doc["max"] = 360;
  doc["unit_of_measurement"] = "minutes";
  doc["availability_topic"] = topic_availability;
  doc["entity_category"] = "config";

  char topic[65];
  homeassistant_generate_topic(topic, "number", "preset_timeout");
  
  serializeJson(doc, buffer);
  client.publish(topic, buffer, true);
}

void homeassistant_autodiscovery_reset_button() {
  DynamicJsonDocument doc(1024);
  homeassistant_autodiscovery_device(doc);
  doc["name"] = "Restart";
  doc["icon"] = "mdi:restart";
  doc["unique_id"] = host_name + "_reset";
  doc["object_id"] = doc["unique_id"];
  doc["command_topic"] = topic_control_reset_button;
  doc["availability_topic"] = topic_availability;
  doc["entity_category"] = "config";

  char topic[65];
  homeassistant_generate_topic(topic, "button", "reset");
  
  serializeJson(doc, buffer);
  client.publish(topic, buffer, true);
}

void homeassistant_autodiscovery() {
  homeassistant_availability();
  homeassistant_autodiscovery_door();
  homeassistant_autodiscovery_light();
  homeassistant_autodiscovery_compressor();
  homeassistant_autodiscovery_compressor_minimum_cycle_duration();
  homeassistant_autodiscovery_preset_timeout();
  homeassistant_autodiscovery_reset_button();

  #if HAS_FRIDGE==true
    homeassistant_autodiscovery_hvac_fridge();
  #endif

  #if HAS_FREEZER==true
    homeassistant_autodiscovery_hvac_freezer();
  #endif
}


unsigned long homeassistant_last_autodiscovery = 0;
void homeassistant_loop() {
  if (millis() - homeassistant_last_autodiscovery > HOMEASSISTANT_AUTODISCOVERY_INTERVAL) {
    homeassistant_autodiscovery();
    homeassistant_last_autodiscovery = millis();
  } 
}

void homeassistant_generate_topic(char *topic_variable, char *component, char *objectid) {
  sprintf(topic_variable, "homeassistant/%s/%s/%s/config", component, host_name.c_str(), objectid);
}
