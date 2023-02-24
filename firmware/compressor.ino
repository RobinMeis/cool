unsigned long compressor_last_change=0;

void compressor_init() {
  pinMode(PIN_COMPRESSOR, OUTPUT);
  digitalWrite(PIN_COMPRESSOR, LOW);
}

void compressor_on() {
  if (!compressor_is_on()) {
    if (compressor_last_change == 0 || (millis() - compressor_last_change) > (compressor_get_minimum_cycle_duration()*60000)) { //Ensure to respect cycle duration
      digitalWrite(PIN_COMPRESSOR, HIGH);
      compressor_last_change = millis();
      compressor_status();
    }
  }
}

void compressor_off() {
  if (compressor_is_on()) {
    if (compressor_last_change == 0 || (millis() - compressor_last_change) > (compressor_get_minimum_cycle_duration()*60000)) { //Ensure to respect cycle duration
      digitalWrite(PIN_COMPRESSOR, LOW);
      compressor_last_change = millis();
      compressor_status();    
    }
  }
}

bool compressor_is_on() {
  return digitalRead(PIN_COMPRESSOR);
}

unsigned int compressor_get_minimum_cycle_duration() {
  return preferences.getUInt("cycleduration", 10);
}

void compressor_set_minimum_cycle_duration(unsigned char duration) {
  if (duration >= 0 && duration <= 30) //Filter disallowed values
    preferences.putUInt("cycleduration", duration);

  compressor_status();
}

void compressor_status() {
  DynamicJsonDocument doc(1024);
  
  doc["on"] = compressor_is_on();
  doc["minimum_cycle_duration"] = compressor_get_minimum_cycle_duration();
 
  serializeJson(doc, buffer);
  client.publish(topic_status_compressor, buffer);
}
