#ifndef BT_CONNECTION_H
#define BT_CONNECTION_H

#include "../base/base.hpp"

class Bt_connection : public virtual Modulebase
{
private:
  IFMV * mFMV;
  void onDisable();
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Bt_connection(byte id, IFMV *fmv) : Modulebase(id, Modules::BT_CONNECTION)  {
    mFMV = fmv;
    setConfig();
  }

  enum Commands : byte {
    CONNECTION_BL_OFF,
    CONNECTION_BL_ON
  };

  enum Events : byte {
    CONFIGURE
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
