
#include "dht.h"
#include "DHT11Driver.hpp"

dht_reading get_humidity_reading(){
    dht_reading reading = {0, 0};

    dht_read_float_data(DHT_TYPE, DHT_PIN, &reading.hum, &reading.temp);
    return reading;
}

// create a wrapper function to get humidity
double sampleHum(){
    dht_reading humidity = get_humidity_reading();

    return humidity.hum;
}
