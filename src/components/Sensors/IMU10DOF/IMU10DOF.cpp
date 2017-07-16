#include "IMU10DOF.h"

IMU10DOF::IMU10DOF()
{
  this->accelgyro = new MPU6050();
  this->i2c = new I2Cdev();
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

  // Serial.println("RAW X,Y,Z:");
  // Serial.print(ax);
  // Serial.print(",");
  // Serial.print(ay);
  // Serial.print(",");
  // Serial.println(az);

  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;


  // Serial.println("Acceleration(g) of X,Y,Z:");
  // Serial.println(Axyz[0]);
  // Serial.print(",");
  // Serial.print(Axyz[1]);
  // Serial.print(",");
  // Serial.println(Axyz[2]);
}
