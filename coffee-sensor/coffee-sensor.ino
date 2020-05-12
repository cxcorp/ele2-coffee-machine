#define ATOMIC_FS_UPDATE
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoWebsockets.h>
#include <PolledTimeout.h>
#include <DoubleResetDetect.h>
#include <HX711.h>
#include "ConfigPersistence.h"
#include "ConfigWebServer.h"
#include "ConfigServerCallbacks.h"
#include "Util.h"
#include "SigningKey.h"
#include "StatusDataProvider.h"
#include "Version.h"

using esp8266::polledTimeout::oneShotMs;

// SCREEN_SCL        D1
// SCREEN_SDA        D2
//                   D3   Boot fails if pulled LOW
//                   D4   Boot fails if pulled LOW, LED_BUILTIN
// LOADCELL_DT       D5
// LOADCELL_CLK      D6
// SCREEN_TOGGLE_PIN D7
//                   D8   Boot fails if pulled HIGH

#define LOADCELL_DT D5
#define LOADCELL_CLK D6

#define SCREEN_TOGGLE_PIN D7
#define SCREEN_SCL D1
#define SCREEN_SDA D2

using namespace websockets;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DRD_TIMEOUT 2
#define DRD_MEM_ADDR 0x00
DoubleResetDetect doubleReset(DRD_TIMEOUT, DRD_MEM_ADDR);

ConfigPersistence configPersistence("/config.json", SPIFFS);
bool isConfigMode = false;
ConfigPersistence::Config appConfig;

HX711 scale;
WebsocketsClient wsClient;

float scaleReading = 0;

// firmware update signing key
BearSSL::PublicKey *signPubKey = nullptr;
BearSSL::HashSHA256 *hash = nullptr;
BearSSL::SigningVerifier *sign = nullptr;

#define FIRMWARE_UPDATE_URL "https://ota.ele2.cxcorp.systems:443/update"
// If the URL is HTTPS, copy its certificate fingerprint below.
// If you set a non-HTTPS URL, comment out the define FIRMWARE_UPDATE_HTTPS line below!
#define FIRMWARE_UPDATE_HTTPS 1
#ifdef FIRMWARE_UPDATE_HTTPS
static const char firmwareUpdateHttpsFingerprint[] PROGMEM = "6F E5 D6 DF FC 00 35 E5 F7 43 92 3F 00 B0 41 0F D1 1E B4 6B";
#endif

StatusDataProvider statusDataProvider(
  FW_VERSION,
  FW_TIMESTAMP,
  &scaleReading,
  &WiFi,
  &ESP,
  &configPersistence
);

ConfigServerCallbacks configServerCallbacks(&configPersistence, &scale);

#define CONFIG_AP_SSID "OH sensor admin"
#define CONFIG_AP_PASSPHRASE "12345678"
const IPAddress configApLocalIp(192, 168, 1, 1);
const IPAddress configApGateway(192, 168, 1, 1);
const IPAddress configApSubnet(255, 255, 255, 0);
ConfigWebServer configServer(80, &statusDataProvider, &configServerCallbacks);

void onFirmwareUpdateError(int err) {
  Serial.printf("Firmware update error: %d\n", err);
  // didn't update - let's get SPIFFS back up
  SPIFFS.begin();
  // wifi & websocket starting should be handled by loop
}

void onFirmwareUpdateProgress(int bytesDownloaded, int totalBytes) {
  // just print updates every 10 blocks or so
  if ((bytesDownloaded / 4096) % 10 == 0 || bytesDownloaded == 0 || bytesDownloaded == totalBytes) {
    Serial.printf("Firmware update progress: %d/%d B\n", bytesDownloaded, totalBytes);
  }
}

void initFirmwareUpdate() {
  // set up pub key verification
  signPubKey = new BearSSL::PublicKey(SigningKey::pubkey);
  hash = new BearSSL::HashSHA256();
  sign = new BearSSL::SigningVerifier(signPubKey);

  Update.installSignature(hash, sign);

  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
  ESPhttpUpdate.onStart([]() {
    // close FS, apparently a good idea in case there are any file handles open
    SPIFFS.end();
    Serial.println("Firmware update starting");
  });
  ESPhttpUpdate.onEnd([]() {
    // device reboots after this step
    Serial.println("Firmware update finished, rebooting");
  });
  ESPhttpUpdate.onError(onFirmwareUpdateError);
  ESPhttpUpdate.onProgress(onFirmwareUpdateProgress);
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

  Serial.println("coffee-sensor version " _FW_VERSION ", built at " _FW_TIMESTAMP);

  Serial.println("Initializing SPIFFS");
  SPIFFS.begin();
  configPersistence.initializeFile();
  Serial.println("SPIFFS initialized");

  initFirmwareUpdate();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Wire.begin(SCREEN_SDA, SCREEN_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  pinMode(SCREEN_TOGGLE_PIN, INPUT_PULLUP);

  if (wasDoubleReset) {
    Serial.println("Configuration mode");
    isConfigMode = true;
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
      WiFi.begin(appConfig.SSID, appConfig.psk);
    }
  }

  scale.begin(LOADCELL_DT, LOADCELL_CLK);
  scale.set_scale(appConfig.scale.multiplier);
  scale.set_offset(appConfig.scale.offset); // zero factor from scale.read_average()

  // let ws client connect to HTTPS
  wsClient.setInsecure();
}

void updateFirmware() {
  // Free up heap for update TLS
  // The TLS ops for the websocket scoop up ~20 KB of heap. We don't have enough heap
  // to have connections open for both the websocket and the updater at the same time.
  // Close the websocket's TLS connection before starting to update.
  wsClient.close();
  delay(50);

#ifdef FIRMWARE_UPDATE_HTTPS
  WiFiClientSecure client;
  client.setFingerprint(firmwareUpdateHttpsFingerprint);
#else
  WiFiClient client;
#endif

  Serial.println("Checking for updates");
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, FIRMWARE_UPDATE_URL, FW_VERSION);
  client.stop();

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}

oneShotMs measurementTimeout(2000);
oneShotMs firmwareUpdateTimeout(60000);
oneShotMs wifiDebugTimeout(2000);
oneShotMs wsDebugTimeout(2000);
oneShotMs wsReconnectTimeout(5000);

void normalModeLoop() {
  if (WiFi.status() != WL_CONNECTED) {
    // wait for auto reconnect by esp firmware
    if (wifiDebugTimeout) {
      Serial.printf("WiFi not connected: %s\n", Util::wlStatusString(WiFi.status()));
      wifiDebugTimeout.reset();
    }
    return;
  }

  if (firmwareUpdateTimeout) {
    updateFirmware();
    firmwareUpdateTimeout.reset();
  }

  if (!wsClient.available()) {
    if (wsDebugTimeout) {
      Serial.println("WS client not available");
      wsDebugTimeout.reset();
    }
    if (wsReconnectTimeout) {
      wsClient.close();
      wsClient.connect(appConfig.websocketURL);
      wsReconnectTimeout.reset();
    }
    return;
  }

  wsClient.poll();

  if (measurementTimeout) {
    Serial.println("Measuring...");

    uint32_t measureStart = millis();
    float measurement;
    if (scale.wait_ready_timeout(2000)) {
      measurement = scale.get_units(3);
      Serial.printf("Measuring took %lums\n", millis() - measureStart);
      measurementTimeout.reset();
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
    Serial.printf("Sending took %lums\n", millis() - now);
  }
}

oneShotMs windowChangeDebounce(200);
oneShotMs windowRefresh(33); // 30 FPS is enough for us
oneShotMs configMeasureTimeout(1000);
uint32_t lastWindowChange = 0;
#define CHANGE_WINDOW_INTERVAL 10000

#define WINDOW_COUNT 2
enum ConfigWindow : uint8_t {
  WIFI_CREDS = 0,
  CALIBRATION = 1
};

ConfigWindow window = ConfigWindow::WIFI_CREDS;

// separate prototype here because Arduino IDE autogenerates function prototypes
// in .ino and places them after the last #include so ConfigWindow is undefined,
// meaning that ConfigWindow hasn't been defined yet there
void drawConfigScreen(ConfigWindow window, float windowProgress);

void drawConfigScreen(ConfigWindow window, float windowProgress) {
  display.clearDisplay();

  if (window == ConfigWindow::CALIBRATION) {
    display.setCursor(0, 0);
    display.printf("Current weight:      %.3f", scaleReading);

    display.setCursor(0, 20);
    display.printf("Scale multiplier:    %.1f", scale.get_scale());

    display.setCursor(0, 40);
    display.printf("Scale offset (tare): %ld", scale.get_offset());
  } else if (window == ConfigWindow::WIFI_CREDS) {
    display.setCursor(0, 0);
    display.printf("SSID:\n%s", CONFIG_AP_SSID);
    display.setCursor(0, 20);
    display.printf("Password:\n%s", CONFIG_AP_PASSPHRASE);
    display.setCursor(0, 40);
    display.printf("Admin panel:\n%s:80", configApLocalIp.toString().c_str());
  }

  display.setCursor(106, 53);
  display.printf("%d/%d", window + 1, WINDOW_COUNT);

  int16_t x,y;
  uint16_t w,h;
  display.getTextBounds("1/2", 106, 53, &x, &y, &w, &h);
  display.fillRect(106, 62, constrain(w * windowProgress, 0, w), 2, WHITE);

  display.display();
}

void configModeLoop() {
  configServer.server().handleClient();

  // read current weight to display on admin page
  if (configMeasureTimeout && scale.wait_ready_timeout(1000)) {
    scaleReading = scale.get_units(4);
    configMeasureTimeout.reset();
  }

  int now = millis();
  if (now - lastWindowChange >= CHANGE_WINDOW_INTERVAL) {
    window = (ConfigWindow)(((uint8_t)window + 1) % WINDOW_COUNT);
    lastWindowChange = now;
  }

  if (windowRefresh) {
    windowRefresh.reset();
    drawConfigScreen(window, (now - lastWindowChange) / (float)CHANGE_WINDOW_INTERVAL);
  }
}

void loop() {
  if (isConfigMode) {
    configModeLoop();
  } else {
    normalModeLoop();
  }
}
