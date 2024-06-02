#include <Arduino.h>
#include "tcrt5k.h"

// Defina os pinos para os cinco canais
int irPins[] = {33, 32, 25, 26, 27};

// Inicializa o sensor TCRT5000 com os pinos definidos
TCRT5000 sensor(irPins, 5);  // 5 sensores, nenhum pino LP definido

void setup() {
    Serial.begin(9600);
}

void loop() {
    for (int i = 0; i < 5; i++) {
        if (sensor.isClose(i)) {
            Serial.print("Objeto próximo no sensor ");
            Serial.println(i + 1);
        } else {
            Serial.print("Nenhum objeto detectado no sensor ");
            Serial.println(i + 1);
        }
    }
    delay(500);  // Aguarda meio segundo antes de verificar novamente
}
