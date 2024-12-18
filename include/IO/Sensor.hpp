#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <Arduino.h>

namespace meltwin {

  struct Sensor {
    explicit Sensor(gpio_num_t _data, float min_real_value = 0.0, float max_real_value = 1.0,
                    gpio_num_t _enb_pin = GPIO_NUM_NC) :
        min_val(min_real_value), max_val(max_real_value), enb_pin(_enb_pin), data_pin(_data) {}

    void setup_sensor() {
      if (enb_pin != GPIO_NUM_NC)
        pinMode(enb_pin, OUTPUT);
      pinMode(data_pin, INPUT_PULLDOWN);
    }

    float read_sensor() {
      // Sleep a bit to let the time to the sensor to intiate
      if (enb_pin != GPIO_NUM_NC) {
        digitalWrite(enb_pin, HIGH);
        sleep(1.0);
      }
      auto measure = static_cast<float>(analogRead(data_pin)) / 1023;
      if (enb_pin != GPIO_NUM_NC) {
        sleep(0.1);
        digitalWrite(enb_pin, LOW);
      }

      // Convert measure to wanted value
      return measure * max_val + (1 - measure) * min_val;
    }

    void cleanup() {
      if (enb_pin != GPIO_NUM_NC)
        digitalWrite(enb_pin, LOW);
    }

  private:
    float min_val, max_val;
    gpio_num_t enb_pin;
    gpio_num_t data_pin;
  };

} // namespace meltwin

#endif
