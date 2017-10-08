#ifndef WiredController_H
#define WiredController_H
#include "../AbstractController.h"
#include <Metro.h>

class WiredController : public AbstractController
{
private:

  // Constraints
  int accelConstraint;
  int brakeConstraint;

  // Security
  int previousReading = 0;
  int sameReadingCount = 0;
  void checkSample(int sample);
public:

  WiredController(AbstractDevice device);

  // Virtual Methods
  bool handleController();
  bool enable();
  bool disable();
};


#endif
