
#include "rmsErr.hpp"
#include <cmath>

double rmsErr(double* feeback, int size, double setpoint){
    double msErr = 0; 
    for(int i = 0; i < size; i++){
        msErr += (feeback[i] - setpoint)*(feeback[i] - setpoint);
    }
    msErr /= size;

    return sqrt(msErr);
}

