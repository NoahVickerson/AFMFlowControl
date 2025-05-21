
#include "servoPWM.hpp"

#include "driver/ledc.h"
#include "hal/gpio_types.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"
#include "esp_log.h"


Servo::Servo(int servo_pin){
    this->servo_pin = servo_pin;
    if(servo_num >= max_servos) {
        ESP_LOGI("error", "not enough servo channels, please add more to unique_channels");
        return;
    }
    servo_channel = unique_channels[servo_num];
    servo_num++;
    initServoPWM();
}

Servo::~Servo(){

}

void Servo::moveServo(double angle){
    if(angle > SERVO_MAX_ANGLE){
        ESP_LOGI("warning", "Servo angle %d greater than maximum %d, capping angle", (int)angle, SERVO_MAX_ANGLE);
        angle = SERVO_MAX_ANGLE;
    }
    if(angle < SERVO_MIN_ANGLE){
        ESP_LOGI("warning", "Servo angle %d less than minimum %d, capping angle", (int)angle, SERVO_MIN_ANGLE);
        angle = SERVO_MIN_ANGLE;
    }


    ESP_LOGI("servo pwm", "Moving servo %d on pin %d to angle: %d", servo_channel, servo_pin, (int)angle);

    // Map angle to pulse width in microseconds
    uint32_t us = SERVO_MIN_US + ((SERVO_MAX_US - SERVO_MIN_US) * angle) / 180;

    // Calculate duty (based on 14-bit resolution and 50Hz PWM)
    uint32_t duty = (us * (1 << 14)) / (1000000 / SERVO_FREQ); // scale to 16-bit

    ledc_set_duty(LEDC_LOW_SPEED_MODE, servo_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, servo_channel);
}

void Servo::initServoPWM(){
    ledc_channel_config_t channel_cfg = {
        .gpio_num = servo_pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = servo_channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
        .flags = {0}
    };

    ledc_timer_config_t timer_cfg = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_14_BIT, // not sure this is correct
        .timer_num = LEDC_TIMER_0,
        .freq_hz = SERVO_FREQ,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };

    ledc_timer_config(&timer_cfg);
    ledc_channel_config(&channel_cfg);
}
