#include "StatusDataProvider.h"

StatusData StatusDataProvider::get() {
  StatusData s;
  s.fwVersion = fwVersion;
  s.fwTimestamp = fwTimestamp;
  s.wifiStatus = wifi->status();
  s.espFreeHeap = esp->getFreeHeap();
  s.scaleReading = *scaleReading;

  if (!configPersistence->read(s.appConfig)) {
    Serial.println("Failed to read config for status data");
    s.appConfig.SSID[0] = '\0';
    s.appConfig.psk[0] = '\0';
    s.appConfig.websocketURL[0] = '\0';
  }

  return s;
}