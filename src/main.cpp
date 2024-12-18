/**
 *  ===========================================================================
 *                      Automated Plant Watering System
 *                     By Meltwin - 2024 (c) MIT Licence
 *  ============================================================================
 */

#include <Arduino.h>
#include "ApiCaller.hpp"
#include "IO/Pump.hpp"
#include "IO/Sensor.hpp"
#include "WifiConnect.hpp"
#include "common.hpp"

#include "datetime.h"
#include "dev_console.h"

// ----------------------------------------------------------------------------
// Parameters
// ----------------------------------------------------------------------------
#define WIFI_SSID "B-DTU Wireless"
#define WIFI_PSW "StudentDK"

// Sensors
#define BATTERY_SENSOR_DATA GPIO_NUM_13
#define PLANT1_SENSOR_ENABLE GPIO_NUM_26
#define PLANT2_SENSOR_ENABLE GPIO_NUM_27
#define PLANT3_SENSOR_ENABLE GPIO_NUM_14
#define PLANTS_SENSOR_DATA GPIO_NUM_25
#define WATER_LEVEL_ENABLE GPIO_NUM_33
#define WATER_LEVEL_DATA GPIO_NUM_32

// Pump
#define PUMP1_PWM_PIN GPIO_NUM_18
#define PUMP2_PWM_PIN GPIO_NUM_19
#define PUMP3_PWM_PIN GPIO_NUM_21
#define PUMP_PWM_CHANNEL 0
#define PUMP_PWM_FREQ 16000
#define PUMP_PWM_RESOLUTION 12

// ----------------------------------------------------------------------------
// Aliases
// ----------------------------------------------------------------------------
using meltwin::APICaller;
using meltwin::InternalErrors;
using meltwin::Pump;
using meltwin::PumpCmd;
using meltwin::Sensor;


bool console = false;

// ----------------------------------------------------------------------------
// Debug Console & Automatic Watering programs
// ----------------------------------------------------------------------------
void run_console() {
  do
    Serial.println("Waiting for next cmd ...");
  while (!meltwin::DevConsole::execute_command());
}

void run_watering() {
  // ============================================
  // I - Reading sensors
  // ============================================
  std::vector<Sensor> sensors{
    Sensor(BATTERY_SENSOR_DATA, 0.0, 6.0),
    Sensor(PLANTS_SENSOR_DATA, 0.0, 1.0, PLANT1_SENSOR_ENABLE),
    Sensor(PLANTS_SENSOR_DATA, 0.0, 1.0, PLANT2_SENSOR_ENABLE),
    Sensor(PLANTS_SENSOR_DATA, 0.0, 1.0, PLANT3_SENSOR_ENABLE),
    Sensor(WATER_LEVEL_DATA, 0.0, 1.0, WATER_LEVEL_ENABLE),
  };
  float values[sensors.size()];
  Serial.println("Reading sensors values");
  for (size_t i = 0; i < sensors.size(); i++) {
    Serial.printf("\t-> Reading sensor %zu ... ", i);
    auto& s = sensors[i];
    s.setup_sensor();
    values[i] = s.read_sensor();
    Serial.printf("%f\n", values[i]);
    s.cleanup();
  }

  // ============================================
  // II - Connect to API
  // ============================================
  Serial.println("Initializing WiFi");
  if (!init_wifi(WIFI_SSID, WIFI_PSW))
    return;

  Serial.println("Authenticating on the API");
  std::string token;
  if (auto code = APICaller::authenticate(token); code != InternalErrors::SUCCESS) {
    Serial.printf("\t-> Couldn't authenticate on API: error %d\n", code);
    return;
  }
  Serial.printf("\t-> The connction token is {%s}\n", token.c_str());

  // Upload sensors values
  Serial.println("Sending sensors data to the API");
  for (size_t i = 0; i < sensors.size(); i++) {
    Serial.printf("\t-> Sending data for sensor %zu\n", i);
    if (auto code = APICaller::sendData(token.c_str(), i, values[i]); code != InternalErrors::SUCCESS)
      Serial.printf("\t  Couldn't send sensor data on API: error %d\n", code);
  }

  // ============================================
  // III - Watering plants
  // ============================================
  std::vector<Pump> pumps{Pump(PUMP1_PWM_PIN, PUMP_PWM_CHANNEL, PUMP_PWM_RESOLUTION)};
  for (size_t i = 0; i < pumps.size(); i++) {
    auto& pump = pumps[i];
    PumpCmd cmd;
    APICaller::getPumpCmd(token.c_str(), i, cmd);

    // Power pump if needed
    if (cmd.time > 0.0 && cmd.pwm > 0.0) {
      Serial.printf("Running pump %zu for %f s at %f % ...\n", i, cmd.time, cmd.pwm);
      pump.setup_pump(PUMP_PWM_FREQ);
      pump.run_pump(cmd);
      pump.stop_pump();
    }
    APICaller::pumpingDone(token.c_str(), i);
  }
}

void wrap_up() {
  Serial.println("Wrapping up ...");

  delay(1000);
  digitalWrite(13, LOW);
  Serial.end();
  delay(1000);
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_DURATION);
  esp_deep_sleep_start();
}

// ----------------------------------------------------------------------------
// Setup
// ----------------------------------------------------------------------------
void setup() {

  // Setup sub classes
  Serial.begin(SERIAL_BAUD_RATE);

  // Setup deep sleep
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_DURATION);

  // Load activity pin
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // // Look for serial connection for preferences dev console
  Serial.printf("Send \"cmd\" to start developer console (%f s)\n", meltwin::DevConsole::START_TIMEOUT / 1000.);
  console = meltwin::DevConsole::wait_for_console_launch();

  (console) ? run_console() : run_watering();
  wrap_up();
}

void loop() {}
