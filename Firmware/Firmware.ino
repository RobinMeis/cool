#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "config.h"

#define SW_VERSION "0.0.6"

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


char topic_availability[35],
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

String host_name;

void setup() {
  delay(2000); //Boot delay
  Serial.begin(115200);

  byte mac_binary[6];
  char mac[13];
  WiFi.macAddress(mac_binary);
  sprintf(mac, "%x%x%x%x%x%x", mac_binary[0], mac_binary[1], mac_binary[2], mac_binary[3], mac_binary[4], mac_binary[5]);
  host_name = "cool-" + (String)mac;

  mqtt_generate_topic(topic_availability, "availability");
  mqtt_generate_topic(topic_status_door, "status/door");
  mqtt_generate_topic(topic_control_light, "control/light");
  mqtt_generate_topic(topic_status_light, "status/light");
  mqtt_generate_topic(topic_control_light_brightness, "control/light/brightness");
  mqtt_generate_topic(topic_status_light_brightness, "status/light/brightness");
  mqtt_generate_topic(topic_status_compressor, "status/compressor");
  mqtt_generate_topic(topic_control_compressor_cycle, "control/compressor/cycle");
  mqtt_generate_topic(topic_status_temperature, "status/temperature");
  mqtt_generate_topic(topic_control_preset_mode, "control/preset_mode");
  mqtt_generate_topic(topic_control_preset_mode_timeout, "control/preset_mode/timeout");
  mqtt_generate_topic(topic_status_preset_mode, "status/preset_mode");
  mqtt_generate_topic(topic_status_fridge, "status/fridge");
  mqtt_generate_topic(topic_control_fridge_mode, "control/fridge/mode");
  mqtt_generate_topic(topic_control_fridge_temperature_low, "control/fridge/temperature/low");
  mqtt_generate_topic(topic_control_fridge_temperature_high, "control/fridge/temperature/high");
  mqtt_generate_topic(topic_status_freezer, "status/freezer");
  mqtt_generate_topic(topic_control_freezer_mode, "control/freezer/mode");
  mqtt_generate_topic(topic_control_freezer_temperature_low, "control/freezer/temperature/low");
  mqtt_generate_topic(topic_control_freezer_temperature_high, "control/freezer/temperature/high");
  mqtt_generate_topic(topic_control_reset_button, "control/reset");

  preferences.begin("cool", false); 
  preset_mode_init();
  compressor_init();
  light_init();
  door_init();
  wifi_init();
  mqtt_init();
  temperature_init();
}

void loop() {
  wifi_loop();
  mqtt_loop();
  preset_mode_loop();
  temperature_loop();
  homeassistant_loop();
  door_loop();
}
