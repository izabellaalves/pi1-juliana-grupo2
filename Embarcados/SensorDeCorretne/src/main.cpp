#include <Arduino.h>

// Defina o pino analógico onde o ACS712 está conectado
const int sensorPin = 35;

// Defina a tensão de referência do Arduino (5V)
const float referenceVoltage = 5.0;

// Defina a sensibilidade do sensor ACS712
// ACS712-05B = 185 mV/A, ACS712-20A = 100 mV/A, ACS712-30A = 66 mV/A
const float sensitivity = 185.0; // para ACS712-05B

// Variáveis para armazenar valores
float sensorValue = 0;
float voltage = 0;
float current = 0;
float power = 0;
float energy = 0;

unsigned long previousMillis = 0;
const long interval = 1000; // 1 segundo

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Ler o valor do sensor
    sensorValue = analogRead(sensorPin);

    // Converter o valor lido para tensão
    voltage = (sensorValue / 4095.0) * referenceVoltage;

    // Calcular a corrente (corrente = (tensão - 2.5V) / sensibilidade)
    current = (voltage - (referenceVoltage / 2)) / (sensitivity / 1000);

    // Calcular a potência (assumindo uma tensão constante de 220V)
    power = current * 5.0;

    // Calcular o consumo de energia (energia = potência * tempo)
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        energy += (power / 3600); // potência em watts e intervalo em segundos
    }

    // Imprimir os valores calculados
    Serial.print("Corrente: ");
    Serial.print(current);
    Serial.print(" A, Potência: ");
    Serial.print(power);
    Serial.print(" W, Energia: ");
    Serial.print(energy);
    Serial.println(" Wh");

    delay(1000); // Esperar 1 segundo antes da próxima leitura
}
