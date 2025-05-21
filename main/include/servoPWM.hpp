
#pragma once 

/* Example usage:
 * Servo myServo(0); // For a servo with gpio pin 0 (the pin thats not connected to power or ground)
 *
 * myServo.moveServo(90); // move the servo to 90 degrees
 */

#include "driver/ledc.h"

constexpr int max_servos = 4;
#define SERVO_MIN_US   500
#define SERVO_MAX_US  2500
#define SERVO_FREQ      50
#define SERVO_MAX_ANGLE 180
#define SERVO_MIN_ANGLE 0

constexpr ledc_channel_t unique_channels[max_servos] = {
   LEDC_CHANNEL_0,
   LEDC_CHANNEL_1, 
   LEDC_CHANNEL_2,
   LEDC_CHANNEL_3,
};

class Servo{
    public:
        Servo(int servo_pin);
        ~Servo();

        void moveServo(double angle);

    private:
        inline static int servo_num;
        int servo_pin;
        ledc_channel_t servo_channel;

        void initServoPWM();
};

