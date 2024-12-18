/** ===========================================================================
 *      Automated Plant Watering System
 *          By Meltwin - 2024 (c) MIT Licence
 *  ============================================================================
 * This code provide an automated way of watering your plants. You can select
 * your activation pump duration as well as your watering period.
 */

#include <Arduino.h>
#include "ApiCaller.hpp"
//
// #include "computer_debug.h"
// #include "pwm_manage.h"
// #include "wifi_sync.h"
// #include "datetime.h"
// #include "dev_console.h"
//
// // ----------------------------------------------------------------------------
// // Program
// // ----------------------------------------------------------------------------
// meltwin::SerialCom *com;  // For debug purpose
// meltwin::PWMManager *pwm; // PWM Interface for the pump
// meltwin::WifiSync *wifi;  // Wifi interface for time synchronisation
// bool console = false;
//
// void run_console()
// {
//     do
//         Serial.println("Waiting for next cmd ...");
//     while (!meltwin::DevConsole::execute_command());
// }
//
// void run_watering()
// {
//     // Check if wifi has been ok
//     if (!wifi->success)
//     {
//         Serial.println("WiFi couldn't get actual time, relaunching system ...");
//         return;
//     }
//
//     meltwin::DateTime next_water = meltwin::DateTime::from_memory(WATER_TIME_PREFIX);
//
//     // Check if time to water plants
//     // Else launch deep sleep
//     if (next_water > wifi->datetime)
//         return;
//
//     // Launch watering sequence
//     auto end = millis() + WATERING_TIME;
//     while (millis() < end)
//     {
//         auto pwm_cmd = pwm->make_command();
//         com->debug_pwm(pwm_cmd);
//         delay(250);
//     }
//
//     // Wrap up and deep sleep
//     auto step = meltwin::DateTime::from_iso(WATER_INTERVAL);
//     pwm->stop_pwm();
//     while (next_water < wifi->datetime)
//     {
//         next_water += step;
//         Serial.printf("Updating time to %s \n", next_water.to_iso_string().c_str());
//     }
//
//     next_water.save_to_memory(WATER_TIME_PREFIX);
// }
//
// void wrap_up()
// {
//     Serial.println("Wrapping up ...");
//     delete com;
//     delete pwm;
//     delete wifi;
//
//     delay(1000);
//     digitalWrite(13, LOW);
//     Serial.end();
//     delay(1000);
//     esp_sleep_enable_timer_wakeup(DEEP_SLEEP_DURATION);
//     esp_deep_sleep_start();
// }
//
// void setup()
// {
//
//     // Setup sub classes
//     com = new meltwin::SerialCom();
//
//     // Setup deep sleep
//     esp_sleep_enable_timer_wakeup(DEEP_SLEEP_DURATION);
//
//     // Load activity pin
//     pinMode(13, OUTPUT);
//     digitalWrite(13, HIGH);
//
//     // // Look for serial connection forpreferences dev console
//     meltwin::DevConsole::count_startups();
//     Serial.printf("Send \"cmd\" to start developer console (%f s)\n", meltwin::DevConsole::START_TIMEOUT / 1000.);
//     console = meltwin::DevConsole::wait_for_console_launch();
//
//     Serial.printf("PWM command is at: %d / %d\n", WATER_PWM, PUMP_MAX_PWM_INPUT);
//
//     if (console)
//         run_console();
//     else
//     {
//         pwm = new meltwin::PWMManager();
//         wifi = new meltwin::WifiSync();
//         if (!wifi->success)
//             esp_restart();
//
//         run_watering();
//     }
//
//     wrap_up();
// }
//
// void loop()
// {
// }

void setup() {}
void loop() {}
