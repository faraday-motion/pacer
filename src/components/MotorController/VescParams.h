#ifndef VescParams_h
#define VescParams_h
#include "datatypes.h"

class VescParams
{
public:
  static VescParams* Instance();

  //Public properties
  mc_values motorValues;


  //Public Methods
  void setMotorValues(mc_values *val);


  //Debug
  void printMotorValues();
private:
  VescParams(){};// Private so that it can not be called
  VescParams(VescParams const&){};// copy constructor is private
  VescParams& operator=(VescParams const&){};//assignment operator is private.
  static VescParams* m_pInstance;
};

#endif
