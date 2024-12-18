#ifndef WIFI_CONNECT_HPP
#define WIFI_CONNECT_HPP

#include <WiFi.h>

namespace {

  bool init_wifi(const char* ssid, const char* password, unsigned long timeout = 5000U) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ...");

    auto end = millis() + timeout;
    auto loop_time = millis();
    while (WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECT_FAILED && loop_time < end) {
      Serial.print(".");
      delay(100);
      loop_time = millis();
    }

    if (WiFi.status() == WL_CONNECT_FAILED || loop_time > end) {
      Serial.println("Couldn't connect to WiFi network ...");
      return false;
    }
    else {
      Serial.println("\nConnected to the WiFi network");
      Serial.print("Local ESP32 IP: ");
      Serial.println(WiFi.localIP());
      return true;
    }
  }

} // namespace

#endif
