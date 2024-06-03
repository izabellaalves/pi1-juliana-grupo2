#include <Arduino.h>
#include "tcrt5k.h"

// Defina os pinos para os cinco sensores
int irPins[] = {33, 32, 25, 26, 27};
TCRT5000 sensor(irPins, 5);                 // 5 sensores, nenhum pino LP definido

// Defina os pinos para os motores
#define MOTOR_IN1 18
#define MOTOR_IN2 5
#define MOTOR_IN3 17 // TX2
#define MOTOR_IN4 16 // RX2

// Variáveis do PID
double Kp = 2.0;
double Ki = 0.0;
double Kd = 1.0;
double setPoint = 0;
double input = 0;
double output = 0;
double integral = 0;
double previous_error = 0;

// Função para controlar os motores
void controlMotors(double pidOutput) {
    int motorSpeed = 255;                   // Velocidade máxima

    if (pidOutput > 0) {
        // Curva para a direita
        analogWrite(MOTOR_IN1, motorSpeed);
        analogWrite(MOTOR_IN2, 0);
        analogWrite(MOTOR_IN3, motorSpeed - pidOutput);
        analogWrite(MOTOR_IN4, 0);
    } else if (pidOutput < 0) {
        // Curva para a esquerda
        analogWrite(MOTOR_IN1, motorSpeed + pidOutput);
        analogWrite(MOTOR_IN2, 0);
        analogWrite(MOTOR_IN3, motorSpeed);
        analogWrite(MOTOR_IN4, 0);
    } else {
        // Seguir em linha reta
        analogWrite(MOTOR_IN1, motorSpeed);
        analogWrite(MOTOR_IN2, 0);
        analogWrite(MOTOR_IN3, motorSpeed);
        analogWrite(MOTOR_IN4, 0);
    }
}

void setup() {
    Serial.begin(9600);

    // Configuração dos pinos dos motores
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
}

void loop() {
    // Leitura dos sensores e cálculo do erro
    int position = 0;
    int numSensorsActive = 0;

    for (int i = 0; i < 5; i++) {
        if (sensor.isClose(i)) {                      // sensor.isClose(i) retorna true quando o sensor detecta a cor preta.
            position += (i - 2) * 10;                 // Calcula a posição relativa ao centro (sensor 3) com peso
            numSensorsActive++;
        }
    }

    // Evitar divisão por zero
    if (numSensorsActive > 0) {
        position /= numSensorsActive;
    }

    // Calcular o PID
    double error = setPoint - position;
    integral += error;
    double derivative = error - previous_error;
    output = Kp * error + Ki * integral + Kd * derivative;
    previous_error = error;

    // Controlar os motores com a saída do PID
    controlMotors(output);

    // Debugging
    Serial.print("Posição: ");
    Serial.print(position);
    Serial.print(" Erro: ");
    Serial.print(error);
    Serial.print(" PID Output: ");
    Serial.println(output);

    delay(100); // Pequeno atraso para estabilidade
}
