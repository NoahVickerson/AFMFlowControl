#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "pid.hpp"
#include "DHT11Driver.hpp"
#include "dht.h"
#include "servoPWM.hpp"
#include "rmsErr.hpp"

extern "C" {void app_main();}

// create the servos
Servo servo0(7);
Servo servo1(9);

//create a wrapper function for the first servo
void setServo0(double angle){
    servo0.moveServo(angle);
}

// want increasing the servo angle to increase humidity
void app_main(){
#ifdef humOnly
    ESP_LOGI("test", "testingHum");    

    while(1){
        dht_reading reading = get_humidity_reading();
        ESP_LOGI("humidity", "hum: %.2f, temp: %.2f", reading.hum, reading.temp);
        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
#endif

#ifndef humOnly

    // create the PID
    PID humidityController(sampleHum, setServo0, rmsErr);

    // configure the PID
    humidityController.config(40, 2000, 0.0001, 0.0001);

    // run the PID in another task
    // xTaskCreate(
    // runPos,
    // "humidity servo0 task",
    // 2048,
    // &humidityController,
    // 1,
    // NULL);
    runPos(&humidityController);

    // run the command line to change setpoint and gain parameters
#endif
}
