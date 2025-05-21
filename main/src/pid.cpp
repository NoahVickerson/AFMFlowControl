
#include "pid.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h" 

PID::PID(double (*input)(void), void (*output)(double param), double (*err)(double* i, int size, double set)):frameIdx(0), frameSize(0) {
    this->input = input;
    this->output = output;
    this->err = err;
}

PID::~PID() {

}

void runPos(void* param){
    ESP_LOGI("task", "Running PID Controller");
    PID* controller = static_cast<PID*>(param);
    if(!controller->configured){
        ESP_LOGI("err", "%s", "must configure pid before running"); 
        return;
    }

    // each input frame, get the input value perform pid, and update output value
    while(1){
        // get input
        double inp = controller->input();
        ESP_LOGI("dht", "humidity: %.2f, setpoint: %2.f", inp, controller->setpoint);

        // calculate error
        // controller->sampleFrame[controller->frameIdx] = inp;
        // if(controller->frameSize < errorWindow) controller->frameSize++;
        // controller->frameIdx = (controller->frameIdx + 1)%errorWindow;
        double error = inp - controller->setpoint;//controller->err(controller->sampleFrame, controller->frameSize, controller->setpoint);

        ESP_LOGI("pid", "error: %.2f", error);
        // calculate pid
        double p = controller->propGain * error;
        controller->integral += error * controller->sampleRate;
        double i = controller->integral * controller->intGain;
        controller->derivative = (controller->prev_err - error)/controller->sampleRate; 
        double d = controller->propGain * controller->derivative;

        double pid = p + i + d; 

        controller->prev_err = error;

        // output calculated pid value
        ESP_LOGI("pid", "output: %.2f", pid);
        controller->output(pid);

        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
}

void PID::config(double sp, double sR, double pG, double iG){
    setpoint = sp;
    sampleRate = sR;
    propGain = pG;
    intGain = iG;
    configured = true;
}

void PID::runVel(double propGain, double intGain){
    // each input frame, get the input value perform pid, and update output value
    while(1){
        // get input
        double inp = input();

        // calculate error
        double error = 0;// err(inp, setpoint);
        
        // calculate pid
        double p = propGain * error;
        integral += error * sampleRate;
        double i = integral * intGain;
        derivative = (prev_err - error)/sampleRate; 
        double d = propGain * derivative;

        double pid = p + i + d; 
        
        prev_err = error;

        // output calculated pid value
        output(pid);
    }
}
