
#pragma once

#define DHT_TYPE DHT_TYPE_DHT11
#define DHT_PIN  GPIO_NUM_5

struct dht_reading {
    float temp;
    float hum;
};

dht_reading get_humidity_reading();
double sampleHum();
