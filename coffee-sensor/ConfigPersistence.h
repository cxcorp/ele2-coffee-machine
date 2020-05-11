#ifndef COFFEE_SENSOR_CONFIG_PERSISTENCE_H
#define COFFEE_SENSOR_CONFIG_PERSISTENCE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <ESP8266WiFi.h>

// From https://arduinojson.org/v6/assistant/
// With a JSON object where the Config struct's ssid is 32 chars, psk is 63 and ws url is 256,
// the calculator says we need 436 bytes. ArduinoJSON suggests extra 10% for safety.
// Rounded up to nearest divisible-by-8 value this is 440.
#define JSON_KEY_COUNT 5
#define JSON_STRING_SPACE 440
#define CONFIG_JSON_SIZE (JSON_OBJECT_SIZE(JSON_KEY_COUNT) + JSON_STRING_SPACE)

#define DEFAULT_SCALE_MULTIPLIER -230000
#define DEFAULT_SCALE_OFFSET -117878

class ConfigPersistence {
public:
  struct Config {
    // 32 + 1
    char SSID[WL_SSID_MAX_LENGTH + 1] = {0};
    // 8-63 for passphrase, 63 + null
    char psk[WL_WPA_KEY_MAX_LENGTH + 1] = {0};
    // 256 + 1
    char websocketURL[DNS_MAX_NAME_LENGTH + 1] = {0};

    struct {
      float multiplier = DEFAULT_SCALE_MULTIPLIER;
      int32_t offset = DEFAULT_SCALE_OFFSET;
    } scale;
  };

  ConfigPersistence(const char *filename = "/creds.bin", fs::FS &fs = SPIFFS)
  : _filename(filename), _fs(fs) {
  }

  void initializeFile();
  boolean read(ConfigPersistence::Config &out);
  boolean write(const ConfigPersistence::Config &config);

private:
  const char *_filename;
  fs::FS &_fs;
};

#endif
