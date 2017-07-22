#include "IMU10DOF.h"

IMU10DOF::IMU10DOF()
{
  //this->accelgyro = new MPU6050();
  this->accelgyro = new MPU9150();
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

void IMU10DOF::getCompass_Data()
{
	I2C_M->writeByte(MPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
	delay(10);
	I2C_M->readBytes(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_XOUT_L, 6, buffer_m);

  mx = ((int16_t)(buffer_m[1]) << 8) | buffer_m[0] ;
	my = ((int16_t)(buffer_m[3]) << 8) | buffer_m[2] ;
	mz = ((int16_t)(buffer_m[5]) << 8) | buffer_m[4] ;

	Mxyz[0] = (double) mx * 1200 / 4096;
	Mxyz[1] = (double) my * 1200 / 4096;
	Mxyz[2] = (double) mz * 1200 / 4096;
}

void IMU10DOF::getHeading()
{
  heading=180*atan2(Mxyz[1],Mxyz[0])/PI;
  if(heading <0) heading +=360;
}

void IMU10DOF::getTiltHeading()
{
  float pitch = asin(-Axyz[0]);
  float roll = asin(Axyz[1]/cos(pitch));

  float xh = Mxyz[0] * cos(pitch) + Mxyz[2] * sin(pitch);
  float yh = Mxyz[0] * sin(roll) * sin(pitch) + Mxyz[1] * cos(roll) - Mxyz[2] * sin(roll) * cos(pitch);
  float zh = -Mxyz[0] * cos(roll) * sin(pitch) + Mxyz[1] * sin(roll) + Mxyz[2] * cos(roll) * cos(pitch);
  tiltheading = 180 * atan2(yh, xh)/PI;
  if(yh<0) tiltheading +=360;
}
