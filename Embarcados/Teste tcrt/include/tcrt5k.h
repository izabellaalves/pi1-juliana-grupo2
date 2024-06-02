#ifndef tcrt5k_h
#define tcrt5k_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class TCRT5000 {
public:
    TCRT5000(int RP[], int numSensors, int LP = -1);
    bool isClose(int sensorIndex);
private:
    int* _rp;
    int _lp;
    int _numSensors;
};

#endif 