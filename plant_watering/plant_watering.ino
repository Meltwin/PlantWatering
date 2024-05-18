/** ===========================================================================
 *      Automated Plant Watering System
 *          By Meltwin - 2024 (c) MIT Licence
 *  ============================================================================
 * This code provide an automated way of watering your plants. You can select
 * your activation pump duration as well as your watering period.
 */

// ----------------------------------------------------------------------------
// Configurations
// ----------------------------------------------------------------------------
// General configuration
#define PUMP_PIN 13 // 5V PWM signal output for controlling the pump
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 10

// DEBUG configurations
#define USE_DEBUG true
#define MANUAL_PWM_IN 26 // For manual test of the PWM with a potentiometer

// ----------------------------------------------------------------------------
// Program
// ----------------------------------------------------------------------------
void setup()
{
    // Define pump PWM output
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PUMP_PIN, PWM_CHANNEL);

    analogSetWidth(PWM_RESOLUTION);
}

uint16_t pwm_cmd = 0;

void loop()
{
    // Compute PWM command

#if USE_DEBUG
    // Control manually the PWM by overriding PWM command
    if (pwm_cmd == 0)
        pwm_cmd = analogRead(MANUAL_PWM_IN);
#endif

    // Apply PWM
    ledcWrite(PWM_CHANNEL, pwm_cmd);
    pwm_cmd = 0; // Reset to 0 for next round

    delay(500);
}