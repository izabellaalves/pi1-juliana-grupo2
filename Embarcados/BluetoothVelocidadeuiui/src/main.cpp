#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Ticker ticker;
unsigned long tempoDecorrido = 0;

// Objeto do sensor MPU6050
MPU6050 mpu6050(Wire);
long timer = 0;

// Função chamada pelo ticker a cada segundo
void incrementarTempo() {
  tempoDecorrido++;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  while (!Serial) {
    ; // Aguarda a inicialização da porta serial
  }

  // Inicializa a comunicação Bluetooth
  if (!SerialBT.begin("ESP32test")) {
    Serial.println("Falha ao iniciar o Bluetooth");
  } else {
    Serial.println("Bluetooth iniciado com sucesso. Pronto para parear");
  }

  // Configura o ticker para chamar a função a cada 1 segundo (1000 ms)
  ticker.attach(1, incrementarTempo);

  // Mensagem inicial para verificar se o setup foi concluído
  Serial.println("Setup concluído");
}

void loop() {
  mpu6050.update();

  // Criar um objeto JSON
  StaticJsonDocument<200> jsonDoc;

  // Adicionar os dados ao objeto JSON
  jsonDoc["trajetoria"] = serialized("[[1, 2], [3, 4], [5, 6]]");
  jsonDoc["consumoEnergetico"] = 100;
  jsonDoc["tempo"] = tempoDecorrido; // Use o tempo decorrido da variável global
  jsonDoc["aceleracaoX"] = mpu6050.getAccX();
  jsonDoc["aceleracaoY"] = mpu6050.getAccY();
  // jsonDoc["aceleracaoZ"] = mpu6050.getAccZ();

  // Serializar o objeto JSON em uma string
  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);

  // Enviar a string JSON pela porta serial
  Serial.println(jsonBuffer);
  SerialBT.println(jsonBuffer);

  // Adicionar qualquer código adicional que você tenha aqui

  // Aguardar um tempo antes de enviar o próximo JSON (opcional)
  delay(20);

  // Verificar se há dados recebidos via Bluetooth
  if (SerialBT.available()) {
    String received = SerialBT.readString();
    Serial.println("Dados recebidos via Bluetooth: " + received);
  }
}

int myFunction(int x, int y) {
  return x + y;
}
