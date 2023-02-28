void fridge_turn_on() {
  preferences.putBool("fridge-on", true);
  fridge_status();
}

void fridge_turn_off() {
  preferences.putBool("fridge-on", false);
  fridge_status();
}

bool fridge_is_on() {
  return preferences.getBool("fridge-on", false);
}

float fridge_get_minimum_temperature() {
  if (preset_none_active())
    return round(preferences.getFloat("fridge-min", FRIDGE_MINIMUM_DEFAULT)*100)/100;
  else if (preset_eco_active())
    return round(preferences.getFloat("fridge-min-eco", FRIDGE_MINIMUM_DEFAULT)*100)/100;
  else if (preset_boost_active())
    return round(preferences.getFloat("fridge-min-boo", FRIDGE_MINIMUM_DEFAULT)*100)/100;
}

float fridge_get_maximum_temperature() {
  if (preset_none_active())
    return round(preferences.getFloat("fridge-max", FRIDGE_MAXIMUM_DEFAULT)*100)/100;
  else if (preset_eco_active())
    return round(preferences.getFloat("fridge-max-eco", FRIDGE_MAXIMUM_DEFAULT)*100)/100;
  else if (preset_boost_active())
    return round(preferences.getFloat("fridge-max-boo", FRIDGE_MAXIMUM_DEFAULT)*100)/100;
}

void fridge_set_minimum(float minimum) {
  if (minimum >= FRIDGE_HVAC_MINIMUM && //Ignore invalid values
      fridge_get_maximum_temperature() - 2 > minimum) { //Enforce minimum delta of 2 deg

    if (preset_none_active())
      preferences.putFloat("fridge-min", minimum);
    else if (preset_eco_active())
      preferences.putFloat("fridge-min-eco", minimum);
    else if (preset_boost_active())
      preferences.putFloat("fridge-min-boo", minimum);
  }

  fridge_status();
}

void fridge_set_maximum(float maximum) {
  if (maximum <= FRIDGE_HVAC_MAXIMUM && //Ignore invalid values
      fridge_get_minimum_temperature() + 2 < maximum) { //Enforce minimum delta of 2 deg
    if (preset_none_active())
      preferences.putFloat("fridge-max", maximum);
    else if (preset_eco_active())
      preferences.putFloat("fridge-max-eco", maximum);
    else if (preset_boost_active())
      preferences.putFloat("fridge-max-boo", maximum);
  }
  
  fridge_status();
}

void fridge_status() {
  DynamicJsonDocument doc(1024);
  JsonObject temperature = doc.createNestedObject("temperature");
  
  doc["on"] = fridge_is_on();
  temperature["min"] = fridge_get_minimum_temperature();
  temperature["max"] = fridge_get_maximum_temperature();
  serializeJson(doc, buffer);
  client.publish(topic_status_fridge, buffer, true);
}
