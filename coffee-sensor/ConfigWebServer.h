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
  float scaleReading;
  ConfigPersistence::Config appConfig;
};

typedef StatusData (*GetStatusDataFn)();
typedef void (*OnWifiCredentialsFn)(const char *ssid, const char *psk);
typedef void (*OnWebsocketUrlFn)(const char *websocketUrl);
typedef void (*OnScaleMultiplierSetFn)(float multiplier);
typedef void (*OnScaleOffsetSetFn)(int32_t offset);
typedef void (*OnScaleTareFn)();

class ConfigWebServer {
public:
  ConfigWebServer(uint16_t port) {
    _port = port;
  }

  void begin();
  void onGetStatusData(GetStatusDataFn getStatusData);
  void onWifiCredentials(OnWifiCredentialsFn onWifiCredentials);
  void onWebsocketUrl(OnWebsocketUrlFn onWebsocketUrl);
  void onScaleMultiplierSet(OnScaleMultiplierSetFn cb);
  void onScaleOffsetSet(OnScaleOffsetSetFn cb);
  void onScaleTare(OnScaleTareFn cb);

  ESP8266WebServer &server();

private:
  uint16_t _port;
  GetStatusDataFn _getStatusData = nullptr;
  OnWifiCredentialsFn _onWifiCredentials = nullptr;
  OnWebsocketUrlFn _onWebsocketUrl = nullptr;
  OnScaleMultiplierSetFn _onScaleMultiplierSet = nullptr;
  OnScaleOffsetSetFn _onScaleOffsetSet = nullptr;
  OnScaleTareFn _onScaleTare = nullptr;

  ESP8266WebServer _server;

  bool requiredHttpPostMethod();
  void redirectToIndex();

  void handleIndex();
  void handleBootstrap();
  void handleApiStatus();
  void handlePersistWifi();
  void handlePersistWs();
  void handlePersistScaleMultiplier();
  void handlePersistScaleOffset();
  void handleScaleTare();
  void handleNotFound();  
};

#endif
