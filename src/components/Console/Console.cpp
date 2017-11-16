#include "Console.h"
#include "components/Utility/Log.h"
#include "components/Controller/AbstractController.h"
#include "components/Controller/WsController/WsController.h"
#include "ArduinoJson.h"
// Global static pointer used to ensure a single instance of the class.
Console* Console::console = nullptr;

// Get Console Instance
Console* Console::Cmd()
{
  if(!console) {
    console = new Console;
  }
  return console;
}

void Console::setFMV(FMV *fmv)
{
  Log::Logger()->write(Log::Level::DEBUG, "Started binding FMV to console");
  if (!this->hasFMV())
    this->_fmv = fmv;

  Log::Logger()->write(Log::Level::DEBUG, "Current FMV Version: " + this->_fmv->version);
  Log::Logger()->write(Log::Level::DEBUG, "Finished binding FMV to console");
}

bool Console::hasFMV()
{
  if (this->_fmv == nullptr)
    return false;
  return true;
}


// Handle Commnad By Group
String Console::handle(unsigned int id, unsigned int command, String data)
{
  switch (command) {

    // Logger Commands
    case LOGGER_ENABLE:
    case LOGGER_DISABLE:
    case LOGGER_ADD_TARGET:
    case LOGGER_REMOVE_TARGET:
    case LOGGER_SET_LEVEL:
       return "{\"id\":\" " + String(id) + "\",\"response\": \"" + this->execLoggerCommand(command, data) + "\"}";
    // Config Commands
    case CONFIG_GET:
    case CONFIG_SET:
      return "{\"id\":\"" + String(id) + "\", \"response\":" + this->execConfigCommand(command, data) + "}";

    // Constroller Commands
    case CTRL_GET_ACTIVE:
    case CTRL_SET_ACTIVE:
    case CTRL_UNSET_ACTIVE:
    case CTRL_GET_ALL:
    case CTRL_REGISTER:
    case CTRL_SENT_INPUT:
      return "{\"id\":\"" + String(id) + "\", \"response\":\"" + this->execControllerCommand(command, data) + "\"}";

    default:
      return "{\"id\":\"" + String(id) + "\", \"response\":\"" + String(command) + " is an unknow command.\"}";
  }
}

// Execute Logger Commands
String Console::execLoggerCommand(unsigned int command, String data)
{
  switch (command) {

    case LOGGER_ENABLE:
      Log::Logger()->write(Log::Level::DEBUG, "Command:: Enable Logger");
      Log::Logger()->enable();
      return "Logger Enabled";
      break;

    case LOGGER_DISABLE:
      Log::Logger()->write(Log::Level::DEBUG, "Command:: Disable Logger");
      Log::Logger()->disable();
      return "Logger Disabled";
      break;

    case LOGGER_ADD_TARGET:
      // TODO:: Return response if the logger target does not exist.
      if(data == "SERIAL_LOG")
        Log::Logger()->addTarget(Log::Target::SERIAL_LOG);
      if(data == "WEBSOCKETS")
        Log::Logger()->addTarget(Log::Target::WEBSOCKETS);
      if(data == "LOG_FILE")
        Log::Logger()->addTarget(Log::Target::LOG_FILE);
      return "Target added.";
      break;

    case LOGGER_REMOVE_TARGET:
      // TODO:: Return response if the logger target does not exist.
      if(data == "SERIAL_LOG")
        Log::Logger()->removeTarget(Log::Target::SERIAL_LOG);
      if(data == "WEBSOCKETS")
        Log::Logger()->removeTarget(Log::Target::WEBSOCKETS);
      if(data == "LOG_FILE")
        Log::Logger()->removeTarget(Log::Target::LOG_FILE);
      return "Target removed.";
      break;

    case LOGGER_SET_LEVEL:
      if(data == "DEBUG")
        Log::Logger()->setLevel(Log::Level::DEBUG);
      if(data == "INFO")
        Log::Logger()->setLevel(Log::Level::INFO);
      if(data == "NOTICE")
        Log::Logger()->setLevel(Log::Level::NOTICE);
      if(data == "WARNING")
        Log::Logger()->setLevel(Log::Level::WARNING);
      if(data == "ERR")
        Log::Logger()->setLevel(Log::Level::ERR);
      return "Level was set.";
      break;

    default:
      return "Unknown Logger Command"; // Very unlikely to happen.
  }
}


String Console::execConfigCommand(unsigned int command, String data)
{
  switch (command) {

    case CONFIG_GET:
      Log::Logger()->write(Log::Level::DEBUG, "Command:: CONFIG_GET");
      return _fmv->configController->getRawConfig();
      break;

    case CONFIG_SET:
      Log::Logger()->write(Log::Level::DEBUG, "Command:: CONFIG_SET");

      if (_fmv->configController->writeRawConfig(data));
        return "Succeeded to write new config.";

      return "Failed to write new config.";
      break;

    default:
      return "Unknown Config Command"; // Very unlikely to happen.
  }
}

String Console::execControllerCommand(unsigned int command, String data)
{
  String registeredControllers;
  switch (command) {

    case CTRL_GET_ALL:
      {
        Log::Logger()->write(Log::Level::DEBUG, "Command:: CTRL_GET_ALL");
        for (byte i = 0; i < 5; i++)
        {
          if (_fmv->controllerManager->registeredControllers[i] != nullptr)
          {
            registeredControllers = registeredControllers +
                                    (String)_fmv->controllerManager->registeredControllers[i]->controller.id[0] + ":" +
                                    (String)_fmv->controllerManager->registeredControllers[i]->controller.id[1] + ":" +
                                    (String)_fmv->controllerManager->registeredControllers[i]->controller.id[2] + ":" +
                                    (String)_fmv->controllerManager->registeredControllers[i]->controller.id[3] + ":" +
                                    (String)_fmv->controllerManager->registeredControllers[i]->controller.id[4] + "|";
          }
        }
        return registeredControllers;
      }
      break;

    case CTRL_UNSET_ACTIVE:
      {
        Log::Logger()->write(Log::Level::DEBUG, "Command:: CTRL_UNSET_ACTIVE");
        if (_fmv->controllerManager->unsetActiveController()) {
          return "Controller was unset";
          break;
        }
        return "Failed to unset active controller";
      }
      break;

    case CTRL_SET_ACTIVE:
      {
        Log::Logger()->write(Log::Level::DEBUG, "Command:: CTRL_SET_ACTIVE");
        byte id[5];
        id[0] = (char)data[0];
        id[1] = (char)data[1];
        id[2] = (char)data[2];
        id[3] = (char)data[3];
        id[4] = (char)data[4];

        if (_fmv->controllerManager->setActiveController(id)) {
          Log::Logger()->write(Log::Level::DEBUG, data + " controller was set as active");
          return data + " controller was set as active";
        }

        Log::Logger()->write(Log::Level::DEBUG, "Failed to set " + data + " controller as active");
        return "Failed to set " + data + " controller as active";
      }
      break;


    case CTRL_GET_ACTIVE:
      {
        Log::Logger()->write(Log::Level::DEBUG, "Command:: CTRL_GET_ACTIVE");
        AbstractController* activeCtrl = _fmv->controllerManager->getActiveControllerId();
        if (activeCtrl != nullptr) {
          return (String)activeCtrl->controller.id[0] + ":" +
                 (String)activeCtrl->controller.id[1] + ":" +
                 (String)activeCtrl->controller.id[2] + ":" +
                 (String)activeCtrl->controller.id[3] + ":" +
                 (String)activeCtrl->controller.id[4];
        }

        return "No active controller";
      }
      break;

    case CTRL_REGISTER: 
      {
        Log::Logger()->write(Log::Level::DEBUG, "Command: CTRL_REGISTER");
        StaticJsonBuffer<800> jsonBuffer;
        JsonObject& json = jsonBuffer.parse(data);
        
        if(!json.success()){
          String error = "Failed to parse controller json object in console.cpp";
          Log::Logger()->write(Log::Level::ERR, error);
          return error;
        }

        // TODO:: Get the num address somehow in here.
        // IDEA:: The num should always be sent as a param. 
        // this way we know whom to respond.

        AbstractDevice pendingDevice;
        String id = json["id"].as<char*>();  
        pendingDevice.id[0] =  id[0]; // Casting const char* to byte[]
        pendingDevice.id[1] =  id[1]; // Casting const char* to byte[]
        pendingDevice.id[2] =  id[2]; // Casting const char* to byte[]
        pendingDevice.id[3] =  id[3]; // Casting const char* to byte[]
        pendingDevice.id[4] =  id[4]; // Casting const char* to byte[]
        pendingDevice.type = json["type"];
        pendingDevice.enabled = json["enabled"];
        pendingDevice.priority = json["priority"];

        if (this->_fmv->controllerManager->registerController(pendingDevice)) {
          return "Controller was registered";
        }

        return "Controller was NOT registerd";
       
      }
      break;

    case CTRL_SENT_INPUT: 
      {  
        AbstractController * activeController = this->_fmv->controllerManager->activeController;
        if(activeController != nullptr && activeController->controller.type == 5)
        {
          StaticJsonBuffer<256> jsonBuffer;
          JsonObject& json = jsonBuffer.parse(data);
          int input = json["input"];
          WsController* wsController = static_cast<WsController *>(activeController);
          wsController->setInput((byte)input);
          return "inputs received.";
        }
        return "Seems like the wsController is not active.";
      }
      break;
  }
}
