#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "config.h"

#define SW_VERSION "0.0.5 hutzi putzi"

Preferences preferences;
WiFiClient espClient;
PubSubClient client(espClient);

#if HAS_FRIDGE==true
  Adafruit_BMP280 bmp_fridge;
  Adafruit_Sensor *bmp_fridge_temp = bmp_fridge.getTemperatureSensor();
#endif

#if HAS_FREEZER==true
  Adafruit_BMP280 bmp_freezer;
  Adafruit_Sensor *bmp_freezer_temp = bmp_freezer.getTemperatureSensor();
#endif

char buffer[2048];
String mac = WiFi.macAddress();
char mac_bytes[13];

char topic_availability[30],
     topic_status_door[40],
     topic_control_light[40],
     topic_status_light[40],
     topic_control_light_brightness[60],
     topic_status_light_brightness[60],
     topic_status_compressor[40],
     topic_control_compressor_cycle[50],
     topic_status_temperature[40],
     topic_control_preset_mode[40],
     topic_control_preset_mode_timeout[50],
     topic_status_preset_mode[40],
     topic_status_fridge[40],
     topic_control_fridge_mode[40],
     topic_control_fridge_temperature_low[60],
     topic_control_fridge_temperature_high[60],
     topic_status_freezer[45],
     topic_control_freezer_mode[45],
     topic_control_freezer_temperature_low[65],
     topic_control_freezer_temperature_high[65],
     topic_control_reset_button[50];

void setup() {
  delay(2000);
  Serial.begin(115200);

  preferences.begin("cool", false); 
  preset_mode_init();
  compressor_init();
  light_init();
  door_init();
  wifi_init();
  mqtt_init();
  temperature_init();

  byte mac_binary[6];
  WiFi.macAddress(mac_binary);
  sprintf(mac_bytes, "%x%x%x%x%x%x", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  mac = mac_bytes;

  sprintf(topic_availability, "fridge_%x%x%x%x%x%x/availability", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_status_door, "fridge_%x%x%x%x%x%x/status/door", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_light, "fridge_%x%x%x%x%x%x/control/light", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_status_light, "fridge_%x%x%x%x%x%x/status/light", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_light_brightness, "fridge_%x%x%x%x%x%x/control/light/brightness", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_status_light_brightness, "fridge_%x%x%x%x%x%x/status/light/brightness", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_status_compressor, "fridge_%x%x%x%x%x%x/status/compressor", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_compressor_cycle, "fridge_%x%x%x%x%x%x/control/compressor/cycle", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_status_temperature, "fridge_%x%x%x%x%x%x/status/temperature", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_preset_mode, "fridge_%x%x%x%x%x%x/control/preset_mode", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_preset_mode_timeout, "fridge_%x%x%x%x%x%x/control/preset_mode/timeout", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_status_preset_mode, "fridge_%x%x%x%x%x%x/status/preset_mode", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_status_fridge, "fridge_%x%x%x%x%x%x/status/fridge", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_fridge_mode, "fridge_%x%x%x%x%x%x/control/fridge/mode", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_fridge_temperature_low, "fridge_%x%x%x%x%x%x/control/fridge/temperature/low", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_fridge_temperature_high, "fridge_%x%x%x%x%x%x/control/fridge/temperature/high", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_status_freezer, "fridge_%x%x%x%x%x%x/status/freezer", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_freezer_mode, "fridge_%x%x%x%x%x%x/control/freezer/mode", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_freezer_temperature_low, "fridge_%x%x%x%x%x%x/control/freezer/temperature/low", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_freezer_temperature_high, "fridge_%x%x%x%x%x%x/control/freezer/temperature/high", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  sprintf(topic_control_reset_button, "fridge_%x%x%x%x%x%x/control/freezer/temperature/high", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
}

void loop() {
  wifi_loop();
  mqtt_loop();
  preset_mode_loop();
  temperature_loop();
  homeassistant_loop();
  door_loop();
}
