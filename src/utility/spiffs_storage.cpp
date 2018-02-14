#include "./spiffs_storage.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/configurator.h"
#include "../logs/logger.h"
#include "../interfaces/isend.h"

void Spiffs_storage::append(String path, const String message){
    //Dont write to the logger in the appendFile method as this would be a cyclic call
    //Serial.println("Appending to file: " + path);
    File file = fs.open(path, FILE_APPEND);
    if(!file){
      file = fs.open(path, FILE_WRITE);
      if(!file){
          //Serial.println("Failed to open file for writing: " + path);
          return;
      }
    }
    if(!file.print(message)){
        //Serial.println(F("Append failed"));
    }
    file.close();
}

void Spiffs_storage::read(String path, ISend* sender){
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Reading file: "), path);
    File file = fs.open(path);
    if(!file || file.isDirectory()){
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to open file for reading: "), path);
      return;
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
