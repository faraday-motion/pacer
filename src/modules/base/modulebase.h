#ifndef MODULEBASE_H
#define MODULEBASE_H
#include <Arduino.h>
#include "../../configuration/default/configuration.h"
#include "../../interfaces/interfaces.hpp"
#include "../../enums/enums.hpp"

class Modulebase : public ILoopable, public IEnableable  {
private:
  byte mId = 0;
  int mModule = Modules::NONE;
  Roles mRole = Roles::MODULE;
protected:
  bool mEnabled = MODULE_DEFAULT_ENABLED;
  void onEnable(){};
  void onDisable(){};
  bool mIsSetup = false;

  virtual void onEvent(byte eventId, bool always = false)
  {

  }

  Modulebase(byte id, int module, Roles role = Roles::MODULE) {
    mId = id;
    mModule = module;
    mRole = role;
  }
public:

    virtual void command(byte command)
    {

    }

    virtual void setConfig()
    {

    }

    virtual void setEnabled(bool enable)
    {
      if (enable)
      {
        onEnable();
        mEnabled = true;
      }
      else
      {
        onDisable();
        mEnabled = false;
      }
    }

    virtual bool enabled()
    {
      return mEnabled;
    }

    int module() const
    {
      return mModule;
    }

    Roles role() const
    {
      return mRole;
    }

    byte id() const
    {
      return mId;
    }
};

#endif
