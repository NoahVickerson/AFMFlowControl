
constexpr int errorWindow = 255;

class PID {
    public:
        // controller config options
        int sampleRate; // in ms
        double setpoint;
        double propGain;
        double intGain;

        // pid specific variables
        double integral;
        double derivative;
        double prev_err;

        // dependencies
        double (*input)(void);
        void (*output)(double);
        double (*err)(double*, int, double);

        // make sure the controller is configured before its run
        bool configured;
        double sampleFrame[errorWindow];
        int frameIdx;
        int frameSize; 

        PID(double (*input)(void), void (*output)(double param), double (*err)(double* i, int size, double set));
        ~PID();


        /* @brief configure the pid controller with different variables
         * @param sp the setpoint of what youre controlling
         * @param sR the sampleRate of the feedback loop in ms (and how often you want to change the output variable
         * @param pG the proportional gain of the controller
         * @param iG the integral gain of the controller
         */
        void config(double sp, double sR, double pG, double iG);

        // velocity-based controller approach
        void runVel(double propGain, double intGain);

    private:
};

/* @brief run the pid controller with the current configuration. Cannot be run before configuring
*/
void runPos(void* param);

