#ifndef CONNECTION_MODULE_H
#define CONNECTION_MODULE_H
#include <Arduino.h>
#include "./base.hpp"
#include "../../vehiclecontrol.h"

class Connection_module : public Modulebase, public IConnection  {
private:
  std::vector<IRecieve*> pRecievers;
protected:
  Connection_module(byte id, int module) : Modulebase(id, module, Roles::CONNECTION_MODULE) {

  }
public:

  std::vector<IRecieve*> recievers() const
  {
    return pRecievers;
  }

  void addReciever(IRecieve * reciever)
  {
    bool hasReciever = false;
    for (int i=0; i<pRecievers.size(); i++)
    {
      if (pRecievers[i] == reciever)
        hasReciever = true;
    }
    if (!hasReciever)
      pRecievers.push_back(reciever);
  }
};

#endif
