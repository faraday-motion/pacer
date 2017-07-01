#ifndef IMU10DOF_H
#define IMU10DOF_H


#include <Arduino.h>
#include "../lib/I2Cdev/I2Cdev.h"
#include "../lib/MPU6050/MPU6050.h"

class IMU10DOF {
MPU6050* accelgyro;
I2Cdev*  i2c; // Note that this might need to be instantiated in a different context.
public:
  IMU10DOF();

  uint8_t buffer_m[6];
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t mx, my, mz;

  float heading;
  float tiltheading;

  float Axyz[3];
  float Gxyz[3];
  float Mxyz[3];

  unsigned int sample_num_mdate = 5000;

  volatile float mx_sample[3];
  volatile float my_sample[3];
  volatile float mz_sample[3];

  float mx_centre = 0;
  float my_centre = 0;
  float mz_centre = 0;

  volatile int mx_max =0;
  volatile int my_max =0;
  volatile int mz_max =0;

  volatile int mx_min =0;
  volatile int my_min =0;
  volatile int mz_min =0;


  void setup();
  void getAccel_Data();
};
#endif
