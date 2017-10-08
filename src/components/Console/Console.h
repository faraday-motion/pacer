#ifndef Console_h
#define Console_h
#include "FMV.h"
#include <Arduino.h>

class FMV; // Forward declaration
class Console {

public:
  // Get signleton instance
  static Console* Cmd();
  void setFMV(FMV* fmv);
  // Handle the received command and call the apropriete exec method.
  String handle(unsigned int command, String data = "");

  // Execute Logger Commands
  String execLoggerCommand(unsigned int command, String data);

  // Execute Config Commands
  String execConfigCommand(unsigned int command, String data);

  // Execute Controller Commands
  String execControllerCommand(unsigned int command, String data);


protected:
  Console(){}; // Constructor is protected
  Console(Console const&){}; // Copy constructor is protected
  Console& operator=(Console const&){}; // Assignment operator is protected

private:
  FMV* _fmv = nullptr;
  static Console* console;
  bool hasFMV();
};

#endif
