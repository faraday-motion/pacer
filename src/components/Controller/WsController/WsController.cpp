#include "WsController.h"
#include "components/Utility/Log.h"

WsController::WsController(WebSocketCommunicator* ws, AbstractDevice device)
  : AbstractController(device)
{
  Log::Logger()->write(Log::Level::DEBUG, "Started Construction of WsController: ");
  this->_ws = ws;
  this->client = device;
  this->connectionLostTimer = new Metro(_LOST_CONNECTION);
  Log::Logger()->write(Log::Level::DEBUG, "Finished Construction of WsController: ");
}

bool WsController::handleController()
{
  processInput(this->input);
  this->input = 50; // In case we lost connection we don't want to just save the last input but rather be safe.
  return true;
}

void WsController::setInput(byte input)
{
  this->input = input;
}

bool WsController::enable()
{
  return true;
}

bool WsController::disable()
{
  return true;
}
