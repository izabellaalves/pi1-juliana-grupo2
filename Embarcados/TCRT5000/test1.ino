int sensor1 = 2; // Sensor connected to pin 2 of the Arduino
int sensor2 = 3;
int sensor3 = 4;
int sensor4 = 5;
int sensor5 = 6;
int switchPin = 7; // PLC (Programmable Logic Controller)
int infrared = 8; // Near

int value1, value2, value3, value4, value5, switchValue, infraredValue;

void setup() 
{
  Serial.begin(9600);

  pinMode(sensor1, INPUT); // Sensor receives signal
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(infrared, INPUT);

}

void loop() 
{  
  value1 = digitalRead(sensor1); // Reads the value from sensor1 and assigns it to the variable value1
  value2 = digitalRead(sensor2);
  value3 = digitalRead(sensor3);
  value4 = digitalRead(sensor4);
  value5 = digitalRead(sensor5);
  switchValue = digitalRead(switchPin);
  infraredValue = digitalRead(infrared);
  
  Serial.print("Sensor 1: ");
  Serial.print(value1); Serial.print("   ");
  Serial.print("Sensor 2: ");
  Serial.print(value2); Serial.print("   ");
  Serial.print("Sensor 3: ");
  Serial.print(value3); Serial.print("   ");
  Serial.print("Sensor 4: ");
  Serial.print(value4); Serial.print("   ");
  Serial.print("Sensor 5: ");
  Serial.print(value5); Serial.print("   ");
  Serial.print("Switch: ");
  Serial.print(switchValue); Serial.print("   ");
  Serial.print("Infrared: ");
  Serial.println(infraredValue);  
  delay(200);

}
