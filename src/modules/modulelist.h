#ifndef MODULELIST_H
#define MODULELIST_H

#include <Arduino.h>
#include <vector>
#include "./base/base.hpp"
#include "./base/control_module.h"
#include "../logs/logger.h"

class ModuleList {
private:
  std::vector<Modulebase*> moduleArray;
public:
  ModuleList() {

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

  void add(Modulebase* fb)
  {
    if (fb != nullptr)
      moduleArray.push_back(fb);
  }

  Modulebase* get(Modules module)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> module() == module)
        return moduleArray[i];
    }
    return nullptr;
  }

  Modulebase* get(byte id)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> id() == id)
        return moduleArray[i];
    }
    return nullptr;
  }

  void command(byte id, byte command)
  {
    Modulebase* m = get(id);
    if (m != nullptr)
      m -> command(command);
  }

  void command(Modules module, byte command)
  {
    Modulebase* m = get(module);
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

  Modulebase* getEnabled(Roles role)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> enabled() && moduleArray[i] -> role() == role)
      {
        return moduleArray[i];
      }
    }
    return nullptr;
  }

  Control_module* getActiveControl()
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> enabled() && moduleArray[i] -> role() == Roles::CONTROL_MODULE)
      {
        Control_module * cm = static_cast<Control_module*>(moduleArray[i]);
        IActive * ma = static_cast<IActive*>(cm);
        if (ma -> isActive())
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
      if (moduleArray[i] -> enabled() && moduleArray[i] -> role() == Roles::CONTROL_MODULE)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("DEACTIVATING MODULE: "), String(moduleArray[i] -> id()));
        Control_module * cm = static_cast<Control_module*>(moduleArray[i]);
        cm -> setActive(false);
      }
    }
  }

  void activate(byte id)
  {
    for (byte i=0; i<moduleArray.size(); i++)
    {
      if (moduleArray[i] -> enabled() && moduleArray[i] -> role() == Roles::CONTROL_MODULE && moduleArray[i] -> id() == id)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("ACTIVATING MODULE: "), String(moduleArray[i] -> id()));
        Control_module * cm = static_cast<Control_module*>(moduleArray[i]);
        cm -> setActive(true);
        break;
      }
    }
  }
};
#endif
