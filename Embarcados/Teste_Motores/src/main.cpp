#include <Arduino.h>

#define MOTOR1_IN1 18  // D18
#define MOTOR1_IN2 5   // D5
#define MOTOR2_IN1 17  // TX2 (GPIO 17)
#define MOTOR2_IN2 16  // RX2 (GPIO 16)

// Configurações de PWM
#define PWM_FREQUENCY 5000 // Frequência do PWM em Hz
#define PWM_RESOLUTION 8   // Resolução do PWM (8 bits significa valores de 0 a 255)

// Inicialização dos canais de PWM
const int motor1_channel1 = 0;
const int motor1_channel2 = 1;
const int motor2_channel1 = 2;
const int motor2_channel2 = 3;

void setup() {
  Serial.begin(115200);

  // Configurar canais de PWM
  ledcSetup(motor1_channel1, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR1_IN1, motor1_channel1);

  ledcSetup(motor1_channel2, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR1_IN2, motor1_channel2);

  ledcSetup(motor2_channel1, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR2_IN1, motor2_channel1);

  ledcSetup(motor2_channel2, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR2_IN2, motor2_channel2);
}

void loop() {
  static unsigned long lastMilli = 0;
  static bool cwDirection = true; // assume initial direction (positive pwm) is clockwise
  static int pwm = 0;

  if (millis() - lastMilli > 50) { // every 50 milliseconds
    if (cwDirection && pwm++ > 255) {  
      cwDirection = false;
    } else if (!cwDirection && pwm-- < 0) {
      cwDirection = true;
    }

    // Atualizando o PWM dos motores
    ledcWrite(motor1_channel1, pwm);
    ledcWrite(motor1_channel2, 255 - pwm); // direção oposta
    ledcWrite(motor2_channel1, pwm);
    ledcWrite(motor2_channel2, 255 - pwm); // direção oposta

    lastMilli = millis();
    Serial.print("Motor 1 PWM: ");
    Serial.println(pwm); // print PWM for motor1
    Serial.print("Motor 2 PWM: ");
    Serial.println(pwm); // print PWM for motor2
  }
}
