/** ===========================================================================
 *      Automated Plant Watering System
 *          By Meltwin - 2024 (c) MIT Licence
 *  ============================================================================
 * This code provide an automated way of watering your plants. You can select
 * your activation pump duration as well as your watering period.
 */

#ifndef MELTWIN_CONFIGS
#define MELTWIN_CONFIGS

#include <Preferences.h>

#include "common.hpp"

#define IN_DEBUG_MODE false

// Debug configuration
#define SERIAL_BAUD_RATE 9600
#define MANUAL_PWM_IN 26 // For manual test of the PWM with a potentiometer

// Deep sleep
#define DEEP_SLEEP_DURATION_S 120
constexpr uint32_t DEEP_SLEEP_DURATION{DEEP_SLEEP_DURATION_S * S_2US};

#endif
