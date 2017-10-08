#ifndef BalanceController_H
#define BalanceController_H
#include "../AbstractController.h"


class BalanceController : public AbstractController
{
private:
  int getBalanceReading();
  int previousReading = 0;
  int sameReadingCount = 0;

  // Constraints
  int brakeConstraint = 200;
  int accelerateConstraint = 650;

// Note that the pin is hardcoded for now to D4/GPIO2
public:
  BalanceController(AbstractDevice device);
  bool handleController();
  bool enable();
  bool disable();
};


#endif
