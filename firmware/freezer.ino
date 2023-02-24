void freezer_turn_on() {
  preferences.putBool("freeze-on", true);
  freezer_status();
}

void freezer_turn_off() {
  preferences.putBool("freeze-on", false);
  freezer_status();
}

bool freezer_is_on() {
  return preferences.getBool("freeze-on", false);
}

float freezer_get_minimum_temperature() {
  if (preset_none_active())
    return round(preferences.getFloat("freeze-min", FREEZER_MINIMUM_DEFAULT)*100)/100;
  else if (preset_eco_active())
    return round(preferences.getFloat("freeze-min-eco", FREEZER_MINIMUM_DEFAULT)*100)/100;
  else if (preset_boost_active())
    return round(preferences.getFloat("freeze-min-boo", FREEZER_MINIMUM_DEFAULT)*100)/100;
}

float freezer_get_maximum_temperature() {
  if (preset_none_active())
    return round(preferences.getFloat("freeze-max", FREEZER_MAXIMUM_DEFAULT)*100)/100;
  else if (preset_eco_active())
    return round(preferences.getFloat("freeze-max-eco", FREEZER_MAXIMUM_DEFAULT)*100)/100;
  else if (preset_boost_active())
    return round(preferences.getFloat("freeze-max-boo", FREEZER_MAXIMUM_DEFAULT)*100)/100;
}

void freezer_set_minimum(float minimum) {
  if (minimum >= FREEZER_HVAC_MINIMUM && //Ignore invalid values
      freezer_get_maximum_temperature() - 2 > minimum) { //Enforce minimum delta of 2 deg

    if (preset_none_active())
      preferences.putFloat("freeze-min", minimum);
    else if (preset_eco_active())
      preferences.putFloat("freeze-min-eco", minimum);
    else if (preset_boost_active())
      preferences.putFloat("freeze-min-boo", minimum);
  }

  freezer_status();
}

void freezer_set_maximum(float maximum) {
  if (maximum <= FREEZER_HVAC_MAXIMUM && //Ignore invalid values
      freezer_get_minimum_temperature() + 2 < maximum) { //Enforce minimum delta of 2 deg
    if (preset_none_active())
      preferences.putFloat("freeze-max", maximum);
    else if (preset_eco_active())
      preferences.putFloat("freeze-max-eco", maximum);
    else if (preset_boost_active())
      preferences.putFloat("freeze-max-boo", maximum);
  }
  
  freezer_status();
}

void freezer_status() {
  DynamicJsonDocument doc(1024);
  JsonObject temperature = doc.createNestedObject("temperature");
  
  doc["on"] = freezer_is_on();
  temperature["min"] = freezer_get_minimum_temperature();
  temperature["max"] = freezer_get_maximum_temperature();
  serializeJson(doc, buffer);
  client.publish(topic_status_freezer, buffer);
}
