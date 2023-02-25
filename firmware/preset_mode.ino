enum Modes {none, eco, boost};
Modes preset_mode=none;
unsigned long preset_mode_last_change;

bool preset_none_active() {
  return preset_mode == none;
}

bool preset_eco_active() {
  return preset_mode == eco;
}

bool preset_boost_active() {
  return preset_mode == boost;
}

void preset_mode_none() {
  preset_mode = none;
  preset_mode_status();
  fridge_status();
  freezer_status();
}

void preset_mode_eco() {
  preset_mode = eco;
  preset_mode_last_change = millis();
  preset_mode_status();
  fridge_status();
  freezer_status();
}

void preset_mode_boost() {
  preset_mode = boost;
  preset_mode_last_change = millis();
  preset_mode_status();
  fridge_status();
  freezer_status();
}

void preset_mode_set_timeout_duration(int duration) {
  if (duration >= 10 && duration <= 120) //Filter disallowed values
    preferences.putUInt("preset-duration", duration);

  preset_mode_status();
}

int preset_mode_get_timeout_duration() {
  return preferences.getUInt("preset-duration", 30);;
}

void preset_mode_init() {
  preset_mode_none();
}

void preset_mode_loop() {
  if (preset_mode != none && millis() - preset_mode_last_change > preset_mode_get_timeout_duration()*60000) {
    preset_mode_none();
  }
}

void preset_mode_status() {
  DynamicJsonDocument doc(1024);
  
  doc["timeout_duration"] = preset_mode_get_timeout_duration();
  
  if (preset_mode == none) {
    doc["mode"] = "none";
  } else if (preset_mode == eco) {
    doc["mode"] = "eco";
  } else if (preset_mode == boost) {
    doc["mode"] = "boost";
  }

  serializeJson(doc, buffer);
  client.publish(topic_status_preset_mode, buffer);
}
