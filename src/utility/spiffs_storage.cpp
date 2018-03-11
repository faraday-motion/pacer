#include "./spiffs_storage.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/configurator.h"
#include "../log/logger.h"
#include "../interfaces/isend.h"

bool Spiffs_storage::save(String path, const String message){
    Logger::Instance().write(LogLevel::INFO, FPSTR("Save: "), path);
    File file = fs.open(path, FILE_WRITE);
    if(!file){
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed open for write: "), path);
      return false;
    }
    if (file.print(message) == 0) {
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to write to file: "), path);
      return false;
    }
    else
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("File write success: "), path);
    }
    file.close();
    return true;
}

bool Spiffs_storage::append(String path, const String message, size_t maxSize){
    File file = fs.open(path, FILE_APPEND);
    if(!file){
      file = fs.open(path, FILE_WRITE);
      if(!file){
          return false;
      }
    }
    Serial.println("File size: " + String(file.size()));
    if (maxSize > 0 && file.size() >= maxSize)
      return false;
    if(!file.print(message)){
      return false;
    }
    file.close();
    return true;
}

bool Spiffs_storage::read(String path, ISend* sender){
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Reading file: "), path);
    File file = fs.open(path);
    if(!file || file.isDirectory()){
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to open file for reading: "), path);
      return false;
    }
    int i = 0;
    while(file.available()) { // && i < 100
      String line = file.readStringUntil('\n');
      sender -> send(line);
      yield();
      i ++;
    }
    file.close();
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Sucessfully read file: "), path);
    return true;
}

bool Spiffs_storage::read(String path, File &file){
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Reading file: "), path);
    file = fs.open(path);
    if(!file){
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to open file for reading: "), path);
      return false;
    }
    return true;
}

bool Spiffs_storage::remove(String path){
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Deleting file: "),  path);
  if(fs.remove(path)){
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("File deleted"));
    return true;
  } else {
    Logger::Instance().write(LogLevel::WARNING, FPSTR("Delete failed"));
    return false;
  }
}
