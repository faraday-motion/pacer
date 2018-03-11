#ifndef MODULELIST_H
#define MODULELIST_H

#include <Arduino.h>
#include <vector>
#include "../interfaces/interfaces.hpp"
#include "./base/base.hpp"
#include "./base/control_module.h"
#include "../log/logger.h"

class ModuleList : public virtual IModuleList{
private:
  std::vector<IModule*> moduleArray;
public:
  ModuleList() : IModuleList(){
  };

  void setup()
  {
    //Setup all the modules
    for (byte i=0; i<moduleArray.size(); i++)
    {
      moduleArray[i] -> setup();
      yield();
    }
  }

  void loop()
  {
    //Run the main loop for the module
    for (byte i=0; i<moduleArray.size(); i++)
    {
      moduleArray[i] -> loop();
      yield();
    }
  }

  void add(IModule * fb)
  {
    if (fb != nullptr)
      moduleArray.push_back(fb);
  }

  IModule * getByType(int module)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> module() == module)
        return moduleArray[i];
    }
    return nullptr;
  }

  IModule * get(byte id)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> id() == id)
        return moduleArray[i];
    }
    return nullptr;
  }

  void commandById(byte id, byte command)
  {
    IModule * m = get(id);
    if (m != nullptr)
      m -> command(command);
  }

  void commandByType(int module, byte command)
  {
    IModule * m = getByType(module);
    if (m != nullptr)
      m -> command(command);
  }

  void disable(byte id)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> id() == id)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("DISABLING MODULE: "), String(moduleArray[i] -> id()));
        moduleArray[i] -> setEnabled(false);
        break;
      }
    }
  }

  void enable(byte id)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> id() == id)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("ENABLING MODULE: "), String(moduleArray[i] -> id()));
        moduleArray[i] -> setEnabled(true);
        break;
      }
    }
  }

  IModule * getEnabledByRole(Roles role)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> enabled() && moduleArray[i] -> role() == (byte)role)
      {
        return moduleArray[i];
      }
    }
    return nullptr;
  }

  void listEnabled()
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> enabled())
        Logger::Instance().write(LogLevel::INFO, FPSTR("ENABLED MODULE: "), moduleArray[i] -> getModuleName() + " " + String(moduleArray[i] -> id()));
    }
  }

  IControlModule * getActiveControl()
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> enabled() && moduleArray[i] -> role() == (byte)Roles::CONTROL_MODULE)
      {
        Modulebase * mb = static_cast<Modulebase*>(moduleArray[i]);
        Control_module * cm = static_cast<Control_module*>(mb);
        //IControlModule * icm = static_cast<IControlModule*>(cc);
        if (cm -> isActive())
          return cm;
      }
    }
    return nullptr;
  }

  void setActiveController(byte id)
  {
    deactivateControllers();
    activate(id);
  }

  void deactivateControllers()
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> enabled() && moduleArray[i] -> role() == (byte)Roles::CONTROL_MODULE)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("DEACTIVATING MODULE: "), String(moduleArray[i] -> id()));
//        IControlModule * cm = static_cast<IControlModule*>(moduleArray[i]);
        Modulebase * mb = static_cast<Modulebase*>(moduleArray[i]);
        Control_module * cm = static_cast<Control_module*>(mb);
        if (cm != nullptr)
          cm -> setActive(false);
      }
    }
  }

  void activate(byte id)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> enabled() && moduleArray[i] -> role() == (byte)Roles::CONTROL_MODULE && moduleArray[i] -> id() == id)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("ACTIVATING MODULE: "), String(moduleArray[i] -> id()));
//        IControlModule * cm = static_cast<IControlModule*>(moduleArray[i]);
        Modulebase * mb = static_cast<Modulebase*>(moduleArray[i]);
        Control_module * cm = static_cast<Control_module*>(mb);
        if (cm != nullptr)
          cm -> setActive(true);
        break;
      }
    }
  }
};
#endif
