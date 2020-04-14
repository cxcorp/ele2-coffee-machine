#ifndef COFFEE_SENSOR_CONFIG_WEB_SERVER_H
#define COFFEE_SENSOR_CONFIG_WEB_SERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <functional>
#include "ConfigPersistence.h"

struct StatusData {
  const char *fwVersion;
  const char *fwTimestamp;
  uint8_t wifiStatus;
  uint32_t espFreeHeap;
  ConfigPersistence::Config appConfig;
};

typedef StatusData (*GetStatusDataFn)();
typedef void (*OnWifiCredentialsFn)(const char *ssid, const char *psk);
typedef void (*OnWebsocketUrlFn)(const char *websocketUrl);

class ConfigWebServer {
public:
  ConfigWebServer(uint16_t port) {
    _port = port;
  }

  void begin();
  void onGetStatusData(GetStatusDataFn getStatusData);
  void onWifiCredentials(OnWifiCredentialsFn onWifiCredentials);
  void onWebsocketUrl(OnWebsocketUrlFn onWebsocketUrl);
  ESP8266WebServer &server();

private:
  uint16_t _port;
  GetStatusDataFn _getStatusData = nullptr;
  OnWifiCredentialsFn _onWifiCredentials = nullptr;
  OnWebsocketUrlFn _onWebsocketUrl = nullptr;
  
  ESP8266WebServer _server;

  void handleIndex();
  void handleBootstrap();
  void handleApiStatus();
  void handlePersistWifi();
  void handlePersistWs();
  void handleNotFound();  
};

#endif
