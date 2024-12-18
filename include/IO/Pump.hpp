#ifndef PUMP_HPP
#define PUMP_HPP

#include "Arduino.h"
#include "common.hpp"

namespace meltwin {

  struct PumpCmd {
    size_t pump_id = 0;
    float time = 0.0;
    unsigned short pwm; // In [0 - 100]
  };

  struct Pump {

    explicit Pump(gpio_num_t _out_pin, unsigned short pwm_channel, unsigned short _resolution) :
        channel(pwm_channel), resolution(_resolution), pwm_pin(_out_pin) {
      max_value = std::pow(2, resolution) - 1;
    }


    void setup_pump(unsigned int freq) {
      ledcSetup(channel, freq, resolution);
      ledcAttachPin(pwm_pin, channel);
    }

    void run_pump(const PumpCmd& cmd) {
      // Convert percentages from API to PWMValue
      PWMValue pump_cmd = (cmd.pwm / 100.0) * max_value;

      // Run the pump
      auto end = millis() + static_cast<unsigned long>(cmd.time * 1000);
      while (millis() < end) {
        ledcWrite(channel, pump_cmd);
        delay(250);
      }
    }

    void stop_pump() {
      ledcWrite(channel, 0);
      ledcDetachPin(pwm_pin);
    }

  private:
    unsigned short channel, resolution;
    PWMValue max_value;
    gpio_num_t pwm_pin;
  };

} // namespace meltwin

#endif
