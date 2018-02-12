#ifndef BT_CONNECTION_H
#define BT_CONNECTION_H

#include "../base/modulebase.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"

class Bt_connection : virtual public Modulebase
{
private:
  FMV *mFMV;
  void onDisable();
protected:
public:
  Bt_connection(byte id, FMV *fmv) : Modulebase(id, Modules::BT_CONNECTION)  {
    mFMV = fmv;
    setConfig();
  }

  enum Commands : byte {
    CONNECTION_BL_OFF,
    CONNECTION_BL_ON
  };

  void setConfig()
  {

  }

  void setup();
  void loop();
  void command(byte command);

};

#endif
