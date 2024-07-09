#include <Arduino.h>
#include <ESP32MX1508.h>
#include <SPIFFS.h>
#include <HTTPClient.h>

/*
    Function declarations
*/

void  readChannels();
void  errorEstimation();
void  speedCalculation();
void  lineFollow();
void  task(void *pvParameters);

void  createLogReport();
void  writeNewReportLine();
bool  removeLogReport();
int   s3SendFile (File fileStream);

uint8_t*  IR_setup(uint8_t channels, uint8_t pins[]);

/*
    Global constants
*/

const uint8_t LED           = 2;

// Configurações do sensor IR
const uint8_t IR_L          = 34;
const uint8_t IR_M          = 35;
const uint8_t IR_R          = 32;
const uint8_t IR_CHANNELS   = 3;

// const uint16_t MAX_READING  = pow(2, SOC_ADC_MAX_BITWIDTH)-1;

// Configurações dos motores
const uint8_t MOTORA_IN1    = 18;  // D18
const uint8_t MOTORA_IN2    = 5;   // D5
const uint8_t MOTORB_IN3    = 17;  // TX2 (GPIO 17)
const uint8_t MOTORB_IN4    = 16;  // RX2 (GPIO 16)

// Inicialização dos canais de PWM
const uint8_t motorA_channel1 = 1;
const uint8_t motorA_channel2 = 2;
const uint8_t motorB_channel3 = 3;
const uint8_t motorB_channel4 = 4;

// Configurações de PWM (PWM_freq * 2^PWM_res < 80 MHz)
const uint8_t  PWM_RESOLUTION = 10;     // Resolução do PWM (10-bits significa valores de 0 a 1024)
const uint16_t PWM_FREQUENCY  = 20000;  // Frequência do PWM em Hz
const uint16_t MAX_DUTY_CYCLE  = (int)(pow(2, PWM_RESOLUTION) - 1);

/*
    Global variables
*/
char _report[] = "/teste.txt";

bool  onLine, onCenter, aligned, stopLine  = true;

uint8_t sensor_counter = 0;
uint16_t max_speed = 1023, min_speed = 770, base_speed = 800, speed_L = 0, speed_R = 0, prev_speed_L = 0, prev_speed_R = 0;

int PID_value = 0;
unsigned long last_millis = 0;

float sensorValues[IR_CHANNELS];
float prev_error = 0.0, error = 0.0;
float Kp = 2.0, Ki = 0.2, Kd = 4.0;
float P = 0.0, I = 0.0, D = 0.0;
float sensorWeights[IR_CHANNELS]  = {-50, 0.1, 50};

// IR setup
uint8_t* sensorArray = IR_setup(IR_CHANNELS, (uint8_t[]){IR_L, IR_M, IR_R});

// Motors setup
MX1508 motorA(MOTORA_IN1, MOTORA_IN2, motorA_channel1, motorA_channel2, PWM_RESOLUTION, PWM_FREQUENCY);
MX1508 motorB(MOTORB_IN3, MOTORB_IN4, motorB_channel3, motorB_channel4, PWM_RESOLUTION, PWM_FREQUENCY);

void setup() {
  // Begin setup
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  Serial.begin(115200);
  while(!Serial){

  }
  Serial.println("Serial iniciado! Conectando ao WiFi...   ");
  WiFi.begin("vinivilelams", "132546987");
  while(WiFi.status() != WL_CONNECTED){

  }
  Serial.println("Conectado!\n\n");
  
  // End setup
  delay(2000);
  createLogReport();
  File reportFile = SPIFFS.open(_report, "r");
  s3SendFile(reportFile);
  removeLogReport();
  createLogReport();
  xTaskCreatePinnedToCore(task, "task", 5120, NULL, 10, NULL, 1);

  digitalWrite(LED, LOW);
  Serial.println("Beginning loop:\n");
}

void loop() {
  // readChannels();
  // printInfo();
  lineFollow();
  // writeNewReportLine();
}

/*
    Function definitions
*/


void    lineFollow(){
  // readChannels();
  errorEstimation();
  speedCalculation();
  writeNewReportLine();
  prev_error = error;
  motorA.motorGo(speed_R);
  motorB.motorGo(speed_L);
}

/*
  Read all sensors and divide value by ADC resolution
*/
void readChannels(){
  sensor_counter = 0;
  for(uint8_t i = 0 ; i < IR_CHANNELS ; i++){
    sensorValues[i] = ((float)analogRead(sensorArray[i]))/(pow(2, SOC_ADC_MAX_BITWIDTH)-1);
    if(sensorValues[i] < 1) sensor_counter++;
    // sensorValues[i] = analogRead(sensorArray[i]);
  }
  delay(10);
}

void    errorEstimation(){
  error     = 0;
  if(sensor_counter) onLine = true;
  else onLine = false;
  
  if (sensorValues[0] == 1 && sensorValues[2] == 1 && sensorValues[1] != 1) onCenter = true;
  else onCenter = false;

  if (sensorValues[0] == 0 && sensorValues[2] == 0 && sensorValues[1] == 0) stopLine = true;
  else stopLine = false;

  if(onLine){
    if(!onCenter){
  // Calculate line error and direction
      for (int i = 0; i < IR_CHANNELS; i++){
        error += sensorValues[i] * sensorWeights[i];
      }
    }
    // else I = 0;
  }
  
  // P =   error;
  // I +=  error*dt;
  // D =   (error - prev_error)/dt;

  P =   error;
  I +=  (error/20);
  D =   error - prev_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  if(stopLine){
    I = 0;
    PID_value = 0; 
  }
}

/*
 Calculate motor speed and drive motors
*/
void  speedCalculation()
{
  // Calculate motor speed from line error
  if(onLine){
    speed_L = base_speed + PID_value;
    speed_R = base_speed - PID_value;
    prev_speed_L = speed_L;
    prev_speed_R = speed_R;
  }
  else{
    speed_L = prev_speed_L;
    speed_R = prev_speed_R;
  }

  // Maximum and minimum constraints
  // if(sensor_counter > 1){
  //   if(speed_R < speed_L){
  //     motorA.motorRev(speed_L);
  //     motorB.motorGo(speed_L);
  //   }
  //   else if(speed_L < speed_R){
  //     motorA.motorGo(speed_R);
  //     motorB.motorRev(speed_R);
  //   }
  //   else{
  //     motorA.motorGo(speed_R);
  //     motorB.motorGo(speed_L);  
  //   }
  // }
  // else{
    if(speed_L > max_speed) speed_L = max_speed;
    if(speed_L < min_speed) speed_L = 0;
    if(speed_R > max_speed) speed_R = max_speed;
    if(speed_R < min_speed) speed_R = 0;
    motorA.motorGo(speed_R);
    motorB.motorGo(speed_L);
  // }

  if(stopLine){
    motorA.motorBrake();
    motorB.motorBrake();
  }
  
}


uint8_t*  IR_setup(uint8_t channels, uint8_t pins[]){
  static uint8_t* IR;
  IR = (uint8_t*)malloc((channels) * sizeof(uint8_t));
  
  for(uint8_t i = 0 ; i < channels ; i++) {
    IR[i] = pins[i];
    pinMode(IR[i], INPUT);
  }
  return IR;
}

void createLogReport()
{
  if (!SPIFFS.begin(true)) 
  {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  if (!SPIFFS.exists(_report)) {
    File reportFile = SPIFFS.open(_report, "w");
    if (reportFile)
    {
      const char header[] = "# -------- Upload of Logs Report --------\n# Time     Filename         Response Code\n";
      reportFile.print(header);

      Serial.println("SPIFFS: Arquivo criado");

      reportFile.close();
    }
  }
}

void writeNewReportLine()
{
  File reportFile = SPIFFS.open(_report, "a");

  if (reportFile && reportFile.size() <= 512000)
  {
    char newLine[1024];
    snprintf(newLine, sizeof(newLine), "millis:%lu | IR:|%.2f|%.2f|%.2f| -- sensors:%d\nonLine?=%s | onCenter?=%s\nError=%.2f | Prev=%.2f| P=%.2f | I=%.2f | D=%.2f\nPID=%d | Left=%d | Right=%d\n\n",
    millis(), sensorValues[0], sensorValues[1], sensorValues[2], sensor_counter, onLine ? "true" : "false", onCenter ? "true" : "false", error, prev_error, P, I, D, PID_value, speed_L, speed_R);

    reportFile.print(newLine);

    Serial.print("SPIFFS: Nova linha escrita no arquivo: ");
    Serial.println(newLine);

    reportFile.close();
  }
}


bool removeLogReport()
{
  return SPIFFS.remove(_report);
}

int s3SendFile (File fileStream)
{

    HTTPClient http;

    char serverUrl[150];
    snprintf(serverUrl, sizeof(serverUrl), "http://192.168.140.190:5000/upload_esp?file_name=teste.txt");

    size_t size = fileStream.size();
    
    if (size <= 0){ 
      return 0; 
    }

    http.begin(serverUrl);

    int httpResponseCode;

    httpResponseCode = http.sendRequest("PUT", &fileStream, size);
    Serial.println("UP: Saiu do http request");
    http.end();

    return httpResponseCode;
}

void  task(void *pvParameters){
  while(1){
    readChannels();
    // speedCalculation();
    // writeNewReportLine();
  }
}