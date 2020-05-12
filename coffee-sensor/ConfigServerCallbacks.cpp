#include "ConfigServerCallbacks.h"
#include "Util.h"

void ConfigServerCallbacks::saveWifiCredentials(const char *ssid, const char *psk) {
    AutoSaveConfig config(configPersistence);
    strlcpy(config.c.SSID, ssid, ARRAY_SIZE(config.c.SSID));
    strlcpy(config.c.psk, psk, ARRAY_SIZE(config.c.psk));
}

void ConfigServerCallbacks::saveWebsocketUrl(const char *websocketUrl) {
    AutoSaveConfig config(configPersistence);
    strlcpy(config.c.websocketURL, websocketUrl, ARRAY_SIZE(config.c.websocketURL));
}

void ConfigServerCallbacks::saveScaleMultiplier(float newMultiplier) {
    scale->set_scale(newMultiplier);

    AutoSaveConfig config(configPersistence);
    config.c.scale.multiplier = newMultiplier;
}

void ConfigServerCallbacks::saveScaleOffset(int32_t newOffset) {
    scale->set_offset(newOffset);

    AutoSaveConfig config(configPersistence);
    config.c.scale.offset = newOffset;
}

void ConfigServerCallbacks::scaleTare() {
    scale->tare();
    int32_t offset = scale->get_offset();
    
    AutoSaveConfig config(configPersistence);
    config.c.scale.offset = offset;
}
