#include <WebServer.h>
#include <HTTPUpdateServer.h>

WebServer httpServer(80);
HTTPUpdateServer httpUpdater;
bool updater_enabled = false;

void updater_init() {
  httpUpdater.setup(&httpServer);
}

void updater_enable() {
  updater_enabled = true;
  httpServer.begin();
  updater_status();
}

void updater_disable() {
  updater_enabled = false;
  httpServer.stop();
  updater_status();
}

void updater_loop() {
  httpServer.handleClient();
}

void updater_status() {
  if (updater_enabled)
    client.publish(topic_status_updater, "ON", true);
  else
    client.publish(topic_status_updater, "OFF", true);
}
