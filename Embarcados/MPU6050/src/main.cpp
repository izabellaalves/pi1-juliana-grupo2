#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

// #define scl D22 
// #define sda D21

void setup() {
    Serial.begin(9600);
    Wire.begin();
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
}

void loop() {
    mpu6050.update();

    Serial.print("AccX: "); Serial.println(mpu6050.getAccX());
    Serial.print("AccY: "); Serial.println(mpu6050.getAccY());
    Serial.print("AccZ: "); Serial.println(mpu6050.getAccZ());

    Serial.print("GyroX: "); Serial.println(mpu6050.getGyroX());
    Serial.print("GyroY: "); Serial.println(mpu6050.getGyroY());
    Serial.print("GyroZ: "); Serial.println(mpu6050.getGyroZ());

    delay(2000);
}
