#ifndef COFFEE_SENSOR_CONFIG_SERVER_CALLBACKS_H
#define COFFEE_SENSOR_CONFIG_SERVER_CALLBACKS_H

#include <stdint.h>
#include <HX711.h>
#include "ConfigPersistence.h"

class ConfigServerCallbacks {
public:
    ConfigServerCallbacks(
        ConfigPersistence *const configPersistence,
        HX711 *const scale
    ): configPersistence(configPersistence),
        scale(scale) {}

    void saveWifiCredentials(const char *ssid, const char *psk);
    void saveWebsocketUrl(const char *websocketUrl);
    void saveScaleMultiplier(float newMultiplier);
    void saveScaleOffset(int32_t newOffset);
    void scaleTare();

private:
    ConfigPersistence *const configPersistence;
    HX711 *const scale;

    class AutoSaveConfig {
    public: 
        AutoSaveConfig(ConfigPersistence *const configPersistence): _configPersistence(configPersistence) {
            c = ConfigPersistence::Config();
            configPersistence->read(c);
        }
        ~AutoSaveConfig() {
            _configPersistence->write(c);
        }

        ConfigPersistence::Config c;
    private:
        ConfigPersistence *const _configPersistence;
    };
};

#endif