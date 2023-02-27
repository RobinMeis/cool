unsigned long wifi_last_reconnect=0;

void wifi_init() { //Initializes wireless connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void wifi_reconnect() { //Checks if wireless network is connected and reconnects if required
  if (WiFi.status() != WL_CONNECTED && millis() - wifi_last_reconnect > 5000) {
    Serial.println("Connecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    wifi_last_reconnect = millis();
  }
}

void wifi_loop() {
  wifi_reconnect();
}
