unsigned long wifi_last_reconnect=0;

void wifi_init() { //Initializes wireless connection
  pinMode(LED_WIFI, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(host_name);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void wifi_reconnect() { //Checks if wireless network is connected and reconnects if required
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_WIFI, HIGH);
  } else {
    digitalWrite(LED_WIFI, LOW);
    if (millis() - wifi_last_reconnect > 5000) {
      Serial.println("Connecting to WiFi...");

      for (int i=0; i<5; ++i) {
        digitalWrite(LED_WIFI, HIGH);
        delay(100);
        digitalWrite(LED_WIFI, LOW);
        delay(100);
      }

      WiFi.disconnect();
      WiFi.reconnect();
      wifi_last_reconnect = millis();
    }
  }
}

void wifi_loop() {
  wifi_reconnect();
}
