#ifndef COFFEE_SENSOR_CONFIG_WEB_SERVER_H
#define COFFEE_SENSOR_CONFIG_WEB_SERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <functional>
#include "ConfigPersistence.h"
#include "ConfigStatusData.h"
#include "StatusDataProvider.h"
#include "ConfigServerCallbacks.h"

class ConfigWebServer {
public:
  ConfigWebServer(
    uint16_t port,
    StatusDataProvider *statusDataProvider,
    ConfigServerCallbacks *callbacks
  ): _port(port), _statusDataProvider(statusDataProvider), _callbacks(callbacks)
  {}

  void begin();
  ESP8266WebServer &server();

private:
  uint16_t _port;
  StatusDataProvider *_statusDataProvider;
  ConfigServerCallbacks *_callbacks;
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
