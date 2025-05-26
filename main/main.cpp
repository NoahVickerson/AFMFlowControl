#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"

#include "pid.hpp"
#include "DHT11Driver.hpp"
#include "dht.h"
#include "servoPWM.hpp"
#include "rmsErr.hpp" 
#include "humSim.hpp"
#include "lcd.hpp"
#include "i2cdev.h"

#define simulate
#define humOnly

extern "C" {void app_main();}

// create the servos
Servo servo0(7);
Servo servo1(9);

// create a humidity simulation
humSim simulation(1.5, 15, 3, 0.5);

//create a wrapper function for the first servo
void setServo0(double angle){
    servo0.moveServo(angle);
}

double get_humidity_sim(){
    return simulation.getHumidity();
}

void update_hum_sim(double i){
    if(i > 40) i = 40;
    if(i < -60) i = -60;
    simulation.update((40 - i)/1.5); 
}

// want increasing the servo angle to increase humidity
void app_main(){
    ESP_ERROR_CHECK(i2cdev_init());
    LCD lcd;

#ifdef humOnly
    ESP_LOGI("test", "testingHum");    
    lcd.print("Humidity        ", 0, 0);
    lcd.print("Temperature     ", 0, 1);

    char hum[16];
    char temp[16];

    while(1){
        dht_reading reading = get_humidity_reading();
        ESP_LOGI("hum", "humidity: %.2f, temp: %.2f", reading.hum, reading.temp);
        snprintf(hum, 16, "%" PRIu32 "  ", (uint32_t) reading.hum); 
        snprintf(temp, 16, "%" PRIu32 "  ", (uint32_t) reading.temp); 
        hum[2] = '%';
        temp[2] = 'C';
        lcd.print(hum, 9, 0);
        lcd.print(temp, 12, 1);
        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
#endif

#ifndef humOnly

    // create the PID
#ifdef simulate
    PID humidityController(get_humidity_sim, update_hum_sim, rmsErr);
    
    // configure the PID
    humidityController.config(40, 500, 0.001, 0.0001);
#else
    PID humidityController(sampleHum, setServo0, rmsErr);

    // configure the PID
    humidityController.config(40, 2000, 0.0001, 0.0001);
#endif


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
