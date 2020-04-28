#include "ConfigPersistence.h"
#include "Util.h"

#define SSID_KEY "ssid"
#define PSK_KEY "psk"
#define WEBSOCKET_URL_KEY "websocket_url"
#define SCALE_MULTIPLIER_KEY "scale_multiplier"
#define SCALE_OFFSET_KEY "scale_offset"

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

  if (doc[SCALE_MULTIPLIER_KEY].isNull()) {
    config.scale.multiplier = DEFAULT_SCALE_MULTIPLIER;
  } else {
    config.scale.multiplier = doc[SCALE_MULTIPLIER_KEY].as<float>();
  }

  if (doc[SCALE_OFFSET_KEY].isNull()) {
    config.scale.offset = DEFAULT_SCALE_OFFSET;
  } else {
    config.scale.offset = doc[SCALE_OFFSET_KEY].as<int32_t>();
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
