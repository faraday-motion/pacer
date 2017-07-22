#ifndef WiredController_H
#define WiredController_H
#include "../AbstractController.h"
#include <Metro.h>

class WiredController : public AbstractController
{

public:

  WiredController(ConfigController* configController, RadioDevice device);
  bool handleController();
  bool enable();
  bool disable();

};


#endif
