unsigned long last_temperature_measurement=0;

void temperature_init() { //Initialize both temperature sensors
  #if HAS_FRIDGE==true
    temperature_init_sensor(bmp_fridge, I2C_ADDRESS_FRIDGE);
  #endif

  #if HAS_FREEZER==true
    temperature_init_sensor(bmp_freezer, I2C_ADDRESS_FREEZER);
  #endif
}

void temperature_init_sensor(Adafruit_BMP280 &sensor, unsigned address) { //Initialize a sensor at the given address
  unsigned status;
  status = sensor.begin(address);
  if (!status) {
    Serial.println("Could not find a valid BMP280 fridge sensor, check wiring or try a different address!");
    while (1) delay(10);
  }

  sensor.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    Adafruit_BMP280::SAMPLING_X1,    /* Pressure oversampling */
    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    Adafruit_BMP280::STANDBY_MS_4000); /* Standby time. */
}

float temperature_read(Adafruit_Sensor *sensor, float offset) {
  sensors_event_t temp_event;
  sensor->getEvent(&temp_event);
  return temp_event.temperature + offset;
}

void temperature_loop() {
  if (millis() - last_temperature_measurement > 5000) {
    DynamicJsonDocument doc(1024);
    float temperature_fridge = 0, temperature_freezer = 0;
    
    #if HAS_FRIDGE==true
      temperature_fridge = temperature_read(bmp_fridge_temp, OFFSET_FRIDGE);
      doc["fridge"] = round(temperature_fridge*100)/100.0;
    #endif

    #if HAS_FREEZER==true
      temperature_freezer = temperature_read(bmp_freezer_temp, OFFSET_FREEZER);
      doc["freezer"] = round(temperature_freezer*100)/100.0;
    #endif

    serializeJson(doc, buffer);
    client.publish(topic_status_temperature, buffer, true);
    Serial.println(buffer);
    
    controller_loop(temperature_fridge, temperature_freezer);
    last_temperature_measurement = millis();
  }
}
