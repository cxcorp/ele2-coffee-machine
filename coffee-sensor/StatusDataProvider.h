#ifndef COFFEE_SENSOR_STATUS_DATA_PROVIDER_H
#define COFFEE_SENSOR_STATUS_DATA_PROVIDER_H

#include "ConfigStatusData.h"
#include "ConfigPersistence.h"

class StatusDataProvider {
public:
  StatusDataProvider(
    const char *const fwVersion,
    const char *const fwTimestamp,
    float *const scaleReading,
    ESP8266WiFiClass *const wifi,
    EspClass *const esp,
    ConfigPersistence *const configPersistence
  ): fwVersion(fwVersion),
    fwTimestamp(fwTimestamp),
    scaleReading(scaleReading),
    wifi(wifi),
    esp(esp),
    configPersistence(configPersistence) {}

  StatusData get();

private:
  const char *const fwVersion;
  const char *const fwTimestamp;
  float *const scaleReading;
  ESP8266WiFiClass *const wifi;
  EspClass *const esp;
  ConfigPersistence *const configPersistence;
};

#endif