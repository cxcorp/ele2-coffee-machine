#ifndef COFFEE_SENSOR_CONFIG_PERSISTENCE_H
#define COFFEE_SENSOR_CONFIG_PERSISTENCE_H

#include <Arduino.h>
#include <FS.h>
#include <ESP8266WiFi.h>

class ConfigPersistence {
public:
  struct Config {
    // 32 + 1
    char SSID[WL_SSID_MAX_LENGTH + 1] = {0};
    // 8-63 for passphrase, 63 + null
    char psk[WL_WPA_KEY_MAX_LENGTH + 1] = {0};
    // 256 + 1
    char websocketURL[DNS_MAX_NAME_LENGTH + 1] = {0};
  };

  ConfigPersistence(const char *filename = "/creds.bin", fs::FS &fs = SPIFFS)
  : _filename(filename), _fs(fs) {
  }

  boolean read(Config &out);
  boolean write(const Config &config);

private:
  const char *_filename;
  fs::FS &_fs;
};

#endif
