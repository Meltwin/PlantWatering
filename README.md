<h1 align="center">
    Automatic plant watering system
</h1>

This project consist of making an automated watering system for my plants. It works with an ESP-32 board that controls a 5V pump. This repository contains:

- The electrical schematic (made in KiCAD) that can be found in the `KiCAD` folder,
- The code for the ESP-32 board in the `plant_watering` folder,

The project was tested and made on a ESP-WROOM-32 board.

Distributed under the MIT licence. Copyright (c) 2024 Meltwin

## Hardware description

The parts used here are:

- an ESP32 board,
- a dual MOFSET driver,
- USB-C female port for power input,
- Barrel Jack female port for power output (for the pump)

## Software description

The software available here control a 5V pump through the dual MOSFET driver with a PWM signal. There two main parameter for controlling the pump behaviour: the **opened time** and the **opened frequency**. This way people can freely choose the quantity of water to actually drop according to the type of plant.

This data will be stored onto the ESP controller internal storage so that it survive power disconnect.

## Online resources used

- [ESP-32 Getting Started - Expressif](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html)
- [ESP-32 Pinout - Last Minute Engineers](https://lastminuteengineers.com/esp32-pinout-reference/)
- [PWN usage - Upesy (FR only)](https://www.upesy.fr/blogs/tutorials/how-to-use-pwm-on-esp32-with-examples)
- [ADC usage - Last Minute Engineers](https://lastminuteengineers.com/esp32-basics-adc/)