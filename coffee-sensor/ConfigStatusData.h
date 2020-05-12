#ifndef COFFEE_SENSOR_CONFIG_STATUS_DATA_H
#define COFFEE_SENSOR_CONFIG_STATUS_DATA_H

#include <stdint.h>
#include "ConfigPersistence.h"

struct StatusData {
  const char *fwVersion;
  const char *fwTimestamp;
  uint8_t wifiStatus;
  uint32_t espFreeHeap;
  float scaleReading;
  ConfigPersistence::Config appConfig;
};

#endif