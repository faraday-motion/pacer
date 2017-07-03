#ifndef WiredController_H
#define WiredController_H
#include "../AbstractController.h"
#include <Metro.h>

class WiredController : public AbstractController
{

public:

  WiredController(ConfigController* configController, RadioDevice device);
  void read();
  void write();
  bool enable();
  bool handleController();

};


#endif
