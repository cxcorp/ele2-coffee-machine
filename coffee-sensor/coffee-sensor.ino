#include <ESP8266WiFi.h>
#include <FS.h>
#include <ArduinoWebsockets.h>
#include <PolledTimeout.h>
#include <DoubleResetDetect.h>
#include <HX711.h>
#include "ConfigPersistence.h"
#include "ConfigWebServer.h"
#include "Util.h"

using esp8266::polledTimeout::oneShotMs;

#define LOADCELL_DT D2
#define LOADCELL_CLK D3
#define TARE_PIN D6

#define SCALE_CALIBRATION -230000

using namespace websockets;

#define DRD_TIMEOUT 2
// RTC memory address
#define DRD_MEM_ADDR 0x00
DoubleResetDetect doubleReset(DRD_TIMEOUT, DRD_MEM_ADDR);

ConfigPersistence configPersistence("/config.json", SPIFFS);

#define CONFIG_AP_SSID "OH sensor admin"
#define CONFIG_AP_PASSPHRASE "12345678"
const IPAddress configApLocalIp(192, 168, 1, 1);
const IPAddress configApGateway(192, 168, 1, 1);
const IPAddress configApSubnet(255, 255, 255, 0);
ConfigWebServer configServer(80);

HX711 scale;
WebsocketsClient wsClient;

bool isConfigMode = false;
ConfigPersistence::Config appConfig;

StatusData configGetStatus() {
  StatusData statusData;
  statusData.wifiStatus = WiFi.status();
  statusData.espFreeHeap = ESP.getFreeHeap();
  if (!configPersistence.read(statusData.appConfig)) {
    statusData.appConfig.SSID[0] = '\0';
    statusData.appConfig.psk[0] = '\0';
    statusData.appConfig.websocketURL[0] = '\0';
  }

  return statusData;
}

void configOnWifiCredentials(const char *ssid, const char *psk) {
  ConfigPersistence::Config config;
  configPersistence.read(config);
  strlcpy(config.SSID, ssid, ARRAY_SIZE(config.SSID));
  strlcpy(config.psk, psk, ARRAY_SIZE(config.psk));
  configPersistence.write(config);
}

void configOnWebsocketUrl(const char *websocketUrl) {
  ConfigPersistence::Config config;
  configPersistence.read(config);
  strlcpy(config.websocketURL, websocketUrl, ARRAY_SIZE(config.websocketURL));
  configPersistence.write(config);
}

void setup() {
  // detect double reset as quickly as possible
  bool wasDoubleReset = doubleReset.detect();

  if (!wasDoubleReset) {
    // Give user a chance to do a double-reset before starting
    // to initialize things
    delay(DRD_TIMEOUT * 1000);
  }

  Serial.begin(115200);
  Serial.println();

  Serial.println("Initializing SPIFFS");
  SPIFFS.begin();
  Serial.println("SPIFFS initialized");

  if (wasDoubleReset) {
    Serial.println("Configuration mode");
    isConfigMode = true;
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    WiFi.mode(WIFI_AP);
    WiFi.disconnect();
    WiFi.setAutoReconnect(false);

    WiFi.softAPConfig(configApLocalIp, configApGateway, configApSubnet);

    Serial.println("Initializing SoftAP");
    if (!WiFi.softAP(CONFIG_AP_SSID, CONFIG_AP_PASSPHRASE)) {
      Serial.println("Failed to begin softAP!");
      while (1) {
        yield();
      }
    }
    Serial.println("SoftAP started");

    Serial.println("Initializing config web server");
    configServer.onGetStatusData(configGetStatus);
    configServer.onWifiCredentials(configOnWifiCredentials);
    configServer.onWebsocketUrl(configOnWebsocketUrl);
    configServer.begin();
    Serial.println("Config web server online");
  } else {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    // enable WiFi client DHCP in case someone disabled it in the past
    // on this board
    IPAddress anyIp(0, 0, 0, 0);
    WiFi.config(anyIp, anyIp, anyIp);

    if (!configPersistence.read(appConfig)) {
      Serial.println("No persisted wifi credentials or failed to read config!");
    } else {
      Serial.printf("Persisted SSID: %s, psk: %s, ws: %s\n", appConfig.SSID, appConfig.psk, appConfig.websocketURL);
      WiFi.begin(appConfig.SSID, appConfig.psk);
    }
  }

  pinMode(TARE_PIN, INPUT_PULLUP);
  scale.begin(LOADCELL_DT, LOADCELL_CLK);
  scale.set_scale(SCALE_CALIBRATION);
  scale.set_offset(-117878); // zero factor from scale.read_average()
  //scale.tare(); //Reset the scale to 0

  // let ws client connect to HTTPS
  wsClient.setInsecure();
}

uint32_t lastTare = 0;
static bool isTare() {
  if (millis() - lastTare > 1000) {
    lastTare = millis();
    return digitalRead(TARE_PIN) == 0;
  }
  return false;
}

uint32_t lastMeasure = 0;
static bool shouldMeasure() {
  if (millis() - lastMeasure > 2000) {
    lastMeasure = millis();
    return true;
  }
  return false;
}

oneShotMs wsReconnectTimeout(2000);

void loop() {
  if (isConfigMode) {
    configServer.server().handleClient();
    return;
  }

  if (WiFi.status() != WL_CONNECTED) {
    // wait for auto reconnect by esp firmware
    return;
  }

  if (!wsClient.available()) {
    if (wsReconnectTimeout) {
      wsClient.connect(appConfig.websocketURL);
      wsReconnectTimeout.reset();
    }
    return;
  }

  if (isTare()) {
    Serial.println("Tare");
    scale.tare();
    yield();
  }

  wsClient.poll();

  if (shouldMeasure()) {
    Serial.println("Measuring...");

    float measurement = 2.54;
    if (scale.wait_ready_timeout(1000)) {
      measurement = scale.get_units(3);
    } else {
      Serial.println("Load cell timed out!");
      return;
    }
    
    char buffer[16] = {0};
    snprintf(buffer, 16, "%.3f", measurement);
    
    Serial.printf("Sending: %s\n", buffer);
    uint32_t now = millis();
    if (!wsClient.send(buffer)) {
      return;
    }
    uint32_t duration = millis() - now;
    Serial.printf("Sending took %ums\n", duration);
  }
}
