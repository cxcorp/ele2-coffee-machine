#ifndef COFFEE_SENSOR_CONFIG_PERSISTENCE_H
#define COFFEE_SENSOR_CONFIG_PERSISTENCE_H

#include <Arduino.h>
#include <FS.h>

class ConfigPersistence {
public:
  struct Config {
    // 32 + null
    char SSID[33] = {0};
    // length is 8-63 for passphrase, 63 + null
    char psk[64] = {0};
    char websocketURL[129] = {0}; // 128 + 1
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
