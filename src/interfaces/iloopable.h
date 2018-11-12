#ifndef ILOOPABLE_H
#define ILOOPABLE_H

class ILoopable {
   public:
      virtual void setup() = 0;
      virtual void loop() = 0;
};
#endif
