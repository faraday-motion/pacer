#ifndef BT_CONNECTION_H
#define BT_CONNECTION_H

#include "../base/base.hpp"

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
    //setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("BT_CONNECTION");
  }
};

#endif
