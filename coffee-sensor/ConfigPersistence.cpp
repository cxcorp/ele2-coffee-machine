#include "ConfigPersistence.h"
#include <ArduinoJson.h>
#include "Util.h"

// From https://arduinojson.org/v6/assistant/
// With a JSON object where the ssid is 32 chars, psk is 63 and ws url is 256, the calculator
// says we need 378 bytes. ArduinoJSON suggests extra 10% for safety.
// Rounded up to nearest divisible-by-4 value this is 416.
#define JSON_KEY_COUNT 3
#define JSON_STRING_SPACE 416
#define CONFIG_JSON_SIZE (JSON_OBJECT_SIZE(JSON_KEY_COUNT) + JSON_STRING_SPACE)

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

  DynamicJsonDocument doc(CONFIG_JSON_SIZE);
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

  DynamicJsonDocument doc(CONFIG_JSON_SIZE);
  doc[SSID_KEY] = config.SSID;
  doc[PSK_KEY] = config.psk;
  doc[WEBSOCKET_URL_KEY] = config.websocketURL;
  doc[SCALE_MULTIPLIER_KEY] = config.scale.multiplier;
  doc[SCALE_OFFSET_KEY]  = config.scale.offset;

  serializeJson(doc, f);

  f.close();
  return true;
}
