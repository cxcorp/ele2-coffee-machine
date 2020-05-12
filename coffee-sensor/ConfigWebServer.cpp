#include "ConfigWebServer.h"
#include <ArduinoJson.h>
#include "ConfigWebAssets.h"
#include "Util.h"

void ConfigWebServer::begin() {
  this->_server.on("/", [=]() { handleIndex(); });
  this->_server.on("/assets/bootstrap.min.css", [=]() { handleBootstrap(); });
  this->_server.on("/api/status", [=]() { handleApiStatus(); });
  this->_server.on("/persist/wifi", [=]() { handlePersistWifi(); });
  this->_server.on("/persist/ws", [=]() { handlePersistWs(); });
  this->_server.on("/persist/scale/multiplier", [=]() { handlePersistScaleMultiplier(); });
  this->_server.on("/persist/scale/offset", [=]() { handlePersistScaleOffset(); });
  this->_server.on("/persist/scale/tare", [=]() { handleScaleTare(); });
  this->_server.onNotFound([=]() { handleNotFound(); });
  this->_server.begin(this->_port);
}

ESP8266WebServer &ConfigWebServer::server() {
  return this->_server;
}

void ConfigWebServer::handleIndex() {
  _server.send_P(
    200,
    PSTR("text/html; charset=utf-8"),
    ConfigWebAssets::index_min_html
  );
  yield();
}

void ConfigWebServer::handleBootstrap() {
  _server.sendHeader("Content-Encoding", "gzip");
  // Tell the browser to cache the CSS for a long time so we don't need to keep sending it
  _server.sendHeader("Cache-Control", "public, max-age=31536000");
  _server.send_P(
    200,
    PSTR("text/css; charset=utf-8"),
    ConfigWebAssets::bootstrap_css_gz,
    ConfigWebAssets::bootstrap_css_gz_len
  );
  yield();
}

void ConfigWebServer::handleApiStatus() {
  // size generated via https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(JSON_OBJECT_SIZE(10) + 528);

  StatusData statusData = _statusDataProvider->get();
  doc["fwVersion"] = statusData.fwVersion;
  doc["fwTimestamp"] = statusData.fwTimestamp;
  doc["wifiStatus"] = Util::wlStatusString(statusData.wifiStatus);
  doc["freeHeap"] = statusData.espFreeHeap;
  doc["currentSavedSSID"] = statusData.appConfig.SSID;
  doc["currentPasswordLen"] = strnlen(statusData.appConfig.psk, ARRAY_SIZE(statusData.appConfig.psk));
  doc["websocketURL"] = statusData.appConfig.websocketURL;
  doc["scaleMultiplier"] = statusData.appConfig.scale.multiplier;
  doc["scaleOffset"] = statusData.appConfig.scale.offset;
  doc["scaleReading"] = statusData.scaleReading;

  String json = "";
  serializeJson(doc, json);
  
  _server.send(200, "application/json; charset=utf-8", json);
  yield();
}

bool ConfigWebServer::requiredHttpPostMethod() {
  if (_server.method() != HTTP_POST) {
    _server.send(405, "text/plain", "Method Not Allowed");
    return true;
  }

  return false;
}

void ConfigWebServer::redirectToIndex() {
  _server.sendHeader("Location", "/");
  _server.send(302);
}

void ConfigWebServer::handlePersistWifi() {
  // don't handle GET or HEAD etc. if someone comes to this page via page refresh
  if (requiredHttpPostMethod()) {
    return;
  }

  char ssid[33] = {0};
  char psk[64] = {0};

  for (uint8_t i = 0; i < _server.args(); i++) {
    if (_server.argName(i) == "wifi.ssid") {
      strlcpy(ssid, _server.arg(i).c_str(), ARRAY_SIZE(ssid));
    } else if (_server.argName(i) == "wifi.password") {
      strlcpy(psk, _server.arg(i).c_str(), ARRAY_SIZE(psk));
    }
  }

  if (ssid[0] == '\0' || psk[0] == '\0') {
    _server.send(400, "text/plain", "SSID or password was empty!");
    return;
  }

  _callbacks->saveWifiCredentials(ssid, psk);

  redirectToIndex();
  yield();
}

void ConfigWebServer::handlePersistWs() {
  if (requiredHttpPostMethod()) {
    return;
  }

  char wsUrl[129] = {0};

  for (uint8_t i = 0; i < _server.args(); i++) {
    if (_server.argName(i) == "ws.url") {
      strlcpy(wsUrl, _server.arg(i).c_str(), ARRAY_SIZE(wsUrl));
      break;
    }
  }

  if (wsUrl[0] == '\0') {
    _server.send(400, "text/plain", "URL must not be empty!");
    return;
  }

  _callbacks->saveWebsocketUrl(wsUrl);

  redirectToIndex();
  yield();
}

void ConfigWebServer::handlePersistScaleMultiplier() {
  if (requiredHttpPostMethod()) {
    return;
  }

  float multiplier = 1.0;
  for (uint8_t i = 0; i < _server.args(); i++) {
    if (_server.argName(i) == "scale.multiplier") {
      sscanf(_server.arg(i).c_str(), "%f", &multiplier);
      break;
    }
  }

  _callbacks->saveScaleMultiplier(multiplier);

  redirectToIndex();
  yield();
}

void ConfigWebServer::handlePersistScaleOffset() {
  if (requiredHttpPostMethod()) {
    return;
  }

  int32_t offset = 0;
  for (uint8_t i = 0; i < _server.args(); i++) {
    if (_server.argName(i) == "scale.offset") {
      sscanf(_server.arg(i).c_str(), "%" PRId32, &offset);
      break;
    }
  }

  _callbacks->saveScaleOffset(offset);

  redirectToIndex();
  yield();
}

void ConfigWebServer::handleScaleTare() {
  if (requiredHttpPostMethod()) {
    return;
  }

  _callbacks->scaleTare();

  redirectToIndex();
  yield();
}

void ConfigWebServer::handleNotFound() {
  _server.send(404, "text/plain", "404 not found");
  yield();
}
