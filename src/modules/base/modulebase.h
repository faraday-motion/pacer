#ifndef MODULEBASE_H
#define MODULEBASE_H
#include <Arduino.h>
#include "../../configuration/default/configuration.h"
#include "../../interfaces/interfaces.hpp"
#include "../../enums/enums.hpp"

class Modulebase : public IModule  {
private:
  byte mId = 0;
  int mModule = Modules::NONE;
  byte mRole = (byte)Roles::MODULE;
protected:
  bool mEnabled = MODULE_DEFAULT_ENABLED;
  virtual void onEnable(){};
  virtual void onDisable(){};
  bool mIsSetup = false;
  virtual void onEvent(byte eventId, bool always = false) = 0;

  Modulebase(byte id, int module, Roles role = Roles::MODULE) {
    mId = id;
    mModule = module;
    mRole = (byte)role;
  }
public:

    virtual void command(byte command) = 0;
    virtual void setConfig() = 0;

    void setEnabled(bool enable)
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

    bool enabled()
    {
      return mEnabled;
    }

    int module()
    {
      return mModule;
    }

    byte role()
    {
      return mRole;
    }

    byte id()
    {
      return mId;
    }

    virtual String getModuleName()
    {
      return FPSTR("NONE");
    }
};

#endif
