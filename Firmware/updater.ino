#include <WebServer.h>
#include <HTTPUpdateServer.h>

WebServer httpServer(80);
HTTPUpdateServer httpUpdater;
bool updater_enabled = false;

void updater_init() {
  httpUpdater.setup(&httpServer);
  httpServer.begin();
}

void updater_loop() {
  httpServer.handleClient();
}