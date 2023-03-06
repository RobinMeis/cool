#ifndef CONFIG_H
#define CONFIG_H

//Offset of temperature sensors
#define OFFSET_FRIDGE 0
#define OFFSET_FREEZER 0

//Wifi configuration
#define WIFI_SSID "IoT"
#define WIFI_PASSWORD "MyPassword"

//MQTT configuration
#define MQTT_SERVER "mqtt.local"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "fridge"
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "password"
#define HOMEASSISTANT_AUTODISCOVERY_INTERVAL 60000

//Device type
#define HAS_FRIDGE true
#define HAS_FREEZER true

//Device name (autogenerated from device type)
#if HAS_FRIDGE==true
  #define DEVICE_NAME "Fridge"
#else
  #define DEVICE_NAME "Freezer"
#endif

//Pins
#define PIN_COMPRESSOR 5
#define PIN_LIGHT 13
#define PIN_DOOR 4
#define PIN_DOOR_ACTIVE_LOW true
#define DOOR_DEBOUNCE_TIME 200

//I2C addresses
#define I2C_ADDRESS_FRIDGE 0x76
#define I2C_ADDRESS_FREEZER 0x77

//Temperatures
#define FRIDGE_HVAC_MINIMUM 1
#define FRIDGE_HVAC_MAXIMUM 20
#define FREEZER_HVAC_MINIMUM -40
#define FREEZER_HVAC_MAXIMUM 0

//Default temperatures
#define FRIDGE_MINIMUM_DEFAULT 4
#define FRIDGE_MAXIMUM_DEFAULT 7
#define FREEZER_MINIMUM_DEFAULT -27
#define FREEZER_MAXIMUM_DEFAULT -4.5

#endif
