#include <Arduino.h>
#include "tcrt5k.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;

// Defina os pinos para os três sensores
int irPins[] = {39, 34, 35};
TCRT5000 sensor(irPins, 3);                 // 3 sensores, nenhum pino LP definido

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

// Instância do MPU6050
Adafruit_MPU6050 mpu;

// Variáveis para o MPU6050
sensors_event_t a, g, temp;

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

    SerialBT.begin("ESP32test");
    Serial.println("pronto pra parear");

    // Configuração dos pinos dos motores
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);

    // Inicializar MPU6050
    if (!mpu.begin()) {
        Serial.println("Falha ao encontrar o MPU6050!");
        while (1);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {

     if (Serial.available()) {
        SerialBT.write(Serial.read());
     }
     if (SerialBT.available()) {
        Serial.write(SerialBT.read());
    }
    delay(20);

    // Leitura dos sensores e cálculo do erro
    int position = 0;
    int numSensorsActive = 0;

    for (int i = 0; i < 3; i++) {
        if (sensor.isClose(i)) {                      // sensor.isClose(i) retorna true quando o sensor detecta a cor preta.
            // position += (i - 2) * 10;                 // Calcula a posição relativa ao centro (sensor 3) com peso
            position += (i - 1) * 10;
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

    // Ler os dados do MPU6050
    mpu.getEvent(&a, &g, &temp);

    // Imprimir dados do MPU6050 para mapear o trajeto
    Serial.print("Acelerômetro X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2 ");
    Serial.print("Y: "); Serial.print(a.acceleration.y); Serial.print(" m/s^2 ");
    Serial.print("Z: "); Serial.print(a.acceleration.z); Serial.print(" m/s^2 ");
    Serial.print(" | Giroscópio X: "); Serial.print(g.gyro.x); Serial.print(" rad/s ");
    Serial.print("Y: "); Serial.print(g.gyro.y); Serial.print(" rad/s ");
    Serial.print("Z: "); Serial.print(g.gyro.z); Serial.print(" rad/s ");
    Serial.println();

    // Debugging
    Serial.print("Posição: ");
    Serial.print(position);
    Serial.print(" Erro: ");
    Serial.print(error);
    Serial.print(" PID Output: ");
    Serial.println(output);

    delay(2000); // Pequeno atraso para estabilidade
}
