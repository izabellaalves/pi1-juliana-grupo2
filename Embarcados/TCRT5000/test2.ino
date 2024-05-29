//Vídeo de onde foi tirado esse código: https://www.youtube.com/watch?v=vvicISV4PUc


#define ir1 A0 // Define ir1 como o pino A0
#define ir2 A1 // Define ir2 como o pino A1
#define ir3 A2 // Define ir3 como o pino A2
#define ir4 A3 // Define ir4 como o pino A3
#define ir5 A4 // Define ir5 como o pino A4

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps

  // Configura os pinos dos sensores IR como entradas
  pinMode(ir1, INPUT); // Configura ir1 como entrada
  pinMode(ir2, INPUT); // Configura ir2 como entrada
  pinMode(ir3, INPUT); // Configura ir3 como entrada
  pinMode(ir4, INPUT); // Configura ir4 como entrada
  pinMode(ir5, INPUT); // Configura ir5 como entrada
}

void loop() {
  // Lê os valores dos sensores
  int s1 = digitalRead(ir1);  // Lê o valor do sensor mais à esquerda (ir1)
  int s2 = digitalRead(ir2);  // Lê o valor do sensor à esquerda (ir2)
  int s3 = digitalRead(ir3);  // Lê o valor do sensor do meio (ir3)
  int s4 = digitalRead(ir4);  // Lê o valor do sensor à direita (ir4)
  int s5 = digitalRead(ir5);  // Lê o valor do sensor mais à direita (ir5)

  // Imprime os valores dos sensores no monitor serial
  Serial.print("IR1: "); // Imprime o texto "IR1: "
  Serial.print(s1); Serial.print("   "); // Imprime o valor de s1 e três espaços
  Serial.print("IR2: "); // Imprime o texto "IR2: "
  Serial.print(s2); Serial.print("   "); // Imprime o valor de s2 e três espaços
  Serial.print("IR3: "); // Imprime o texto "IR3: "
  Serial.print(s3); Serial.print("   "); // Imprime o valor de s3 e três espaços
  Serial.print("IR4: "); // Imprime o texto "IR4: "
  Serial.print(s4); Serial.print("   "); // Imprime o valor de s4 e três espaços
  Serial.print("IR5: "); // Imprime o texto "IR5: "
  Serial.println(s5);  // Imprime o valor de s5 e uma nova linha

  delay(200); // Aguarda 200 milissegundos antes de ler novamente
}
