#include "IMU10DOF.h"

IMU10DOF::IMU10DOF()
{
  this->accelgyro = new MPU6050();
  //this->accelgyro = new MPU9150();
  this->I2C_M = new I2Cdev();
}

void IMU10DOF::setup()
{
  Wire.begin(); // In the future we need to check if Wire was already begun.
  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro->initialize();

  // verify connection
  Serial.println("Testing device connections...");
	Serial.println(accelgyro->testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  delay(1000);
}

void IMU10DOF::getAccel_Data()
{
  accelgyro->getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;
}
