
#ifndef IVESC_INTERFACE_H
#define IVESC_INTERFACE_H
#include "./vesc_datatypes.h"

class IVesc_interface
{
  public:
    IVesc_interface()
    {
    }
    
    virtual void setValues(mc_values *values) = 0;
    virtual void print(char *str) = 0;
    virtual void setFirmware(int major, int minor) = 0;
    virtual void setRotorPos(float pos) = 0;
    virtual void setMotorConfiguration(mc_configuration *conf) = 0;
    virtual void setAppConfiguration(app_configuration *conf) = 0;
    virtual void setDetect(float cycle_int_limit, float coupling_k, const signed char *hall_table, signed char hall_res) = 0;
    virtual void setPPM(float val, float ms) = 0;
    virtual void setADC(float val, float voltage) = 0;
    virtual void setChuk(float val) = 0;
    virtual void motorConfigurationRecieved() = 0;
    virtual void appConfigurationRecieved() = 0;
};

#endif
