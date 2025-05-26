#pragma once

class humSim{
    private: 
        double humidity;
        double flowRate;
        double volume;
        double initialConc;
        double timeslice;
        double iVconst;
    public:
        humSim(double flow, double vol, double initConc, double time);
        double getHumidity();
        void update(double concentration);
};

