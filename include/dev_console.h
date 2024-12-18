/** ===========================================================================
 *      Automated Plant Watering System
 *          By Meltwin - 2024 (c) MIT Licence
 *  ============================================================================
 * This code provide an automated way of watering your plants. You can select
 * your activation pump duration as well as your watering period.
 */

#ifndef MELTWIN_DEV_CONSOLE
#define MELTWIN_DEV_CONSOLE

#include <nvs_flash.h>
#include "datetime.h"
#include "hardware_configs.h"

namespace meltwin {
  struct DevConsole {
    static constexpr long int START_TIMEOUT{3000}; // Time windows where one can launch the console, in milliseconds
    static constexpr long int MSG_TIMEOUT{25000};  // Timeout between message, if expired restart the system
    static constexpr size_t MSG_LENGTH{128};       // The max length for a message on the console
    static constexpr char CMD_END{'\n'};           // The end character for the message
    static constexpr char STR_END{'\0'};           // The end of a string in a char buffer

    struct Msgs {
      static constexpr const char* CONSOLE_START{"cmd"};
      static constexpr const char* PING_CMD{"ping"};
      static constexpr const char* EXIT_CMD{"exit"};
      static constexpr const char* SET_WATER_DATE{"setwd"};
      static constexpr const char* CLEAN_CMD{"clean"};
    };

    /**
     * Wait for a "cmd" message on startup to launch a dev' console
     * @return true if the console session should continue, false if the system should go in "work" mode
     */
    static bool wait_for_console_launch() {
      char* buffer = new char[MSG_LENGTH];
      if (!wait_for_next_msg(buffer, START_TIMEOUT)) {
        return false;
      }

      // Check message
      if (auto n = strnlen(buffer, MSG_LENGTH); n > 0 && strncmp(buffer, Msgs::CONSOLE_START, n) == 0) {
        Serial.println("Starting in dev console mode ...");
        return true;
      }
      return false;
    }

    /**
     * Wait and execute the next command
     * @return true if the console should continue to be up, false if the system should reboot
     */
    static bool execute_command() {
      char* buffer = new char[MSG_LENGTH];
      if (!wait_for_next_msg(buffer, MSG_TIMEOUT)) {
        Serial.println("Timed out while waiting for a new message ...");
        return false;
      }

      if (strncmp(buffer, Msgs::PING_CMD, 4) == 0)
        Serial.println("[DEBUG] Ping received !");
      else if (strncmp(buffer, Msgs::EXIT_CMD, 4) == 0)
        return false;
      else if (strncmp(buffer, Msgs::SET_WATER_DATE, 5) == 0)
        // set_watering_date(buffer);
        ;
      else if (strncmp(buffer, Msgs::CLEAN_CMD, 5) == 0)
        clean_nvs();
      else
        Serial.println("[DEBUG] Unknown command ...");
      return true;
    }

  private:
    /**
     * Wait for the next message and read it.
     * @param buffer the char buffer where to store the received command (should just be a pointer, the value will be
     * rewritten)
     * @param timout the timeout in milliseconds for this message
     * @return true if a message has successfully been read, false otherwise
     */
    static bool wait_for_next_msg(char* buffer, unsigned long int timeout) {
      auto end = millis() + timeout;
      buffer[0] = STR_END;
      size_t buffer_index = 0;

      // Read data
      while (millis() < end) {
        if (Serial.available() > 0) {
          auto len = Serial.readBytesUntil(CMD_END, buffer, MSG_LENGTH);
          buffer[len] = STR_END;
          Serial.print("[DEBUG] Received message: \"");
          Serial.print(buffer);
          Serial.println("\"");
          return true;
        }
      }

      return false;
    }

    static void clean_nvs() {
      nvs_flash_erase(); // erase the NVS partition and...
      nvs_flash_init();  // initialize the NVS partition.
      Serial.println("Cleaned the NVS!");
    }
  };
} // namespace meltwin

#endif
