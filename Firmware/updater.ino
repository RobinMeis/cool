#if HTTP_UPDATE_ENABLED==true
  
  #include <WebServer.h>
  #include <HTTPUpdateServer.h>
  
  WebServer httpServer(80);
  HTTPUpdateServer httpUpdater;
  bool updater_enabled = false;
  
  void updater_init() {
    httpUpdater.setup(&httpServer, "/update", HTTP_UPDATE_USERNAME, HTTP_UPDATE_PASSWORD);
    httpServer.begin();
  }
  
  void updater_loop() {
    httpServer.handleClient();
  }

#endif
