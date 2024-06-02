#include "tcrt5k.h"

TCRT5000::TCRT5000(int RP[], int numSensors, int LP) {
    _rp = RP;
    _numSensors = numSensors;
    _lp = LP;

    for (int i = 0; i < numSensors; i++) {
        pinMode(_rp[i], INPUT);
    }
    if (_lp != -1) {
        pinMode(_lp, OUTPUT);
    }
}

bool TCRT5000::isClose(int sensorIndex) {
    if (sensorIndex < 0 || sensorIndex >= _numSensors) {
        return false;                                 // índice inválido
    }

    if (_lp == -1) {
        return !digitalRead(_rp[sensorIndex]);        // Apenas verifica o estado do sensor
    } else {
        bool status;
        digitalWrite(_lp, HIGH);                      // Liga o LED
        delayMicroseconds(100);                       // Aguarda 100 microsegundos para estabilização
        status = !digitalRead(_rp[sensorIndex]);      // Lê o estado do sensor
        digitalWrite(_lp, LOW);                       // Desliga o LED
        return status;
    }
}
