#ifndef ICONNECTION_H
#define ICONNECTION_H

#include <Arduino.h>
#include "./isend.h"

class IConnection : public virtual ISend {
   public:
      virtual void addReciever(IRecieve * reciever) = 0;
};
#endif
