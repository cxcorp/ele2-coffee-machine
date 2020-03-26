#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include "HX711.h"

#define LOADCELL_DT D2
#define LOADCELL_CLK D3
#define TARE_PIN D6

#define SCALE_CALIBRATION -230000

const char* WIFI_SSID = "<ssid>";
const char* WIFI_PASSWORD = "<password>";
const char* WS_URI = "wss://<host>:443/<path>";

using namespace websockets;

HX711 scale;
WebsocketsClient wsClient;

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(TARE_PIN, INPUT_PULLUP);
  scale.begin(LOADCELL_DT, LOADCELL_CLK);
  scale.set_scale(SCALE_CALIBRATION);
  scale.set_offset(-117878); // zero factor from scale.read_average()
  scale.tare(); //Reset the scale to 0

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to wifi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected!");

  Serial.println("Connecting to WS");
  wsClient.setInsecure();
  wsClient.connect(WS_URI);
  Serial.println("Connected!");
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

void loop() {
  wsClient.poll();

  if (isTare()) {
    Serial.println("Tare");
    scale.tare();
    yield();
  }

  if (shouldMeasure()) {
    Serial.println("Measuring...");

    float measurement = 0;
    if (scale.wait_ready_timeout(1000)) {
      measurement = scale.get_units(3);
    } else {
      Serial.println("Load cell timed out!");
      return;
    }
    
    char buffer[16] = {0};
    sprintf(buffer, "%.3f", measurement);
    
    Serial.printf("Sending: %s\n", buffer);
    uint32_t now = millis();
    while (!wsClient.send(buffer)) {
      Serial.println("Reconnecting ws...");
      wsClient.connect(WS_URI);
      Serial.println("Reconnected!");
    }
    uint32_t duration = millis() - now;
    Serial.printf("Sending took %ums\n", duration);
  }
}
