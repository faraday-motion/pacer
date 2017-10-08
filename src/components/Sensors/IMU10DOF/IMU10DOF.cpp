#include "IMU10DOF.h"
#include "components/Utility/Log.h"

IMU10DOF::IMU10DOF()
{
  Log::Logger()->write(Log::Level::DEBUG, "Started Construction of the IMU10DOF.");
  this->accelgyro = new MPU6050();
  this->I2C_M = new I2Cdev();
  Log::Logger()->write(Log::Level::DEBUG, "Finished Construction of the IMU10DOF.");
}

void IMU10DOF::setup()
{
  Log::Logger()->write(Log::Level::DEBUG, "Setting up the IMU10DOF.");

  Wire.begin(); // In the future we need to check if Wire was already begun.

  // initialize device
  accelgyro->initialize();

  delay(1000);
  Log::Logger()->write(Log::Level::DEBUG, "Finished setting up the IMU10DOF.");
}

void IMU10DOF::getAccel_Data()
{
  accelgyro->getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;
}
