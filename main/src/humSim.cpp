
#include "humSim.hpp"
#include <cmath>
#include <numbers>

humSim::humSim(double flow, double vol, double initConc, double time):flowRate(flow), volume(vol), initialConc(initConc), timeslice(time) {
    humidity = initConc*volume;
}

double humSim::getHumidity(){
    return humidity;
}

void humSim::update(double concentration){
    iVconst = humidity + volume*timeslice; // not sure if this is correct
    humidity = flowRate*concentration*timeslice - volume*timeslice + iVconst*pow(std::numbers::e, -1*flowRate*timeslice*concentration/volume);
}

