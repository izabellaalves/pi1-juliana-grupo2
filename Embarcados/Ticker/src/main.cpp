#include <Arduino.h>
#include <Ticker.h>

// Variável global para armazenar o tempo decorrido
unsigned long tempoDecorrido = 0;
Ticker ticker;

// Função chamada pelo ticker a cada segundo
void incrementarTempo() {
  tempoDecorrido++;
  Serial.println("Tempo decorrido: " + String(tempoDecorrido) + " segundos");
}

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);
  while (!Serial) {
    ; // Aguarda a inicialização da porta serial
  }

  // Configura o ticker para chamar a função a cada 1 segundo (1000 ms)
  ticker.attach(1, incrementarTempo);

  // Mensagem inicial para verificar se o setup foi concluído
  Serial.println("Setup concluído");
}

void loop() {
  // O loop principal pode fazer outras coisas sem ser bloqueado
  // pois o ticker lida com a contagem de tempo

  // Adiciona um pequeno atraso no loop para facilitar a visualização da saída serial
  delay(100);
}
