#include "ConfigPersistence.h"
#include <ArduinoJson.h>
#include "Util.h"

#define SSID_KEY "ssid"
#define PSK_KEY "psk"
#define WEBSOCKET_URL_KEY "websocket_url"

boolean ConfigPersistence::read(Config &config) {
  config.SSID[0] = '\0';
  config.psk[0] = '\0';
  config.websocketURL[0] = '\0';

  if (!this->_fs.exists(this->_filename)) {
    return false;
  }

  File f = this->_fs.open(this->_filename, "r");
  if (!f || !f.available()) {
    return false;
  }

  // From https://arduinojson.org/v6/assistant/
  // With our limits, we only need 250 bytes for the strings but let's limit it to +10% to be on the safe side
  StaticJsonDocument<JSON_OBJECT_SIZE(3) + 288> doc;
  DeserializationError err = deserializeJson(doc, f);

  if (err) {
    f.close();
    Serial.printf("Config deserialization failed with error %s\n", err.c_str());
    return false;
  }

  if (doc[SSID_KEY] != nullptr) {
    strlcpy(config.SSID, doc[SSID_KEY], ARRAY_SIZE(config.SSID));
  }

  if (doc[PSK_KEY] != nullptr) {
    strlcpy(config.psk, doc[PSK_KEY], ARRAY_SIZE(config.psk));
  }

  if (doc[WEBSOCKET_URL_KEY] != nullptr) {
    strlcpy(config.websocketURL, doc[WEBSOCKET_URL_KEY], ARRAY_SIZE(config.websocketURL));
  }

  f.close();
  return true;
}

boolean ConfigPersistence::write(const Config &config) {
  this->_fs.remove(this->_filename);

  File f = this->_fs.open(this->_filename, "w");
  if (!f) {
    return false;
  }

  StaticJsonDocument<JSON_OBJECT_SIZE(3) + 288> doc;
  doc[SSID_KEY] = config.SSID;
  doc[PSK_KEY] = config.psk;
  doc[WEBSOCKET_URL_KEY] = config.websocketURL;

  serializeJson(doc, f);

  f.close();
  return true;
}
