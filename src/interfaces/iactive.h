#ifndef IACTIVE_H
#define IACTIVE_H

class IActive {
   public:
      virtual void setActive(bool active) = 0;
      virtual bool isActive() = 0;
};
#endif
