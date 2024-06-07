#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Ticker.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Ticker ticker;
unsigned long tempoDecorrido = 0;

// Função chamada pelo ticker a cada segundo
void incrementarTempo() {
  tempoDecorrido++;
  Serial.println("Tempo decorrido: " + String(tempoDecorrido) + " segundos");
}

void setup() {
  Serial.begin(115200);
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
  // Criar um objeto JSON
  StaticJsonDocument<200> jsonDoc;

  // Adicionar os dados ao objeto JSON
  jsonDoc["trajetoria"] = "";
  jsonDoc["tempo"] = tempoDecorrido; // Use o tempo decorrido da variável global
  jsonDoc["velocidade"] = 56.9;
  jsonDoc["aceleracao"] = 98.9;
  jsonDoc["consumoEnergetico"] = 230;

  // Serializar o objeto JSON em uma string
  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);

  // Enviar a string JSON pela porta serial
  Serial.println(jsonBuffer);
  SerialBT.println(jsonBuffer);

  // Adicionar qualquer código adicional que você tenha aqui

  // Aguardar um tempo antes de enviar o próximo JSON (opcional)
  delay(1000);

  // Verificar se há dados recebidos via Bluetooth
  if (SerialBT.available()) {
    String received = SerialBT.readString();
    Serial.println("Dados recebidos via Bluetooth: " + received);
  }
}

int myFunction(int x, int y) {
  return x + y;
}