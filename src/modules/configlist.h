#ifndef CONFIGLIST_H
#define CONFIGLIST_H

#include <Arduino.h>
#include <vector>
#include <algorithm>
#include "./base/modulebase.h"
#include "../utility/spiffs_config.h"
#include "../utility/spiffs_storage.h"
#include "../utility/logger_writer.h"
#include "../configuration/base/configbase.h"
#include "../configuration/base/default_configbase.h"
#include "../factory/configfactory.h"

class Configlist {
private:
  std::vector<Configbase*> mConfigArray;
  Spiffs_config mSpiffs_config;
  Spiffs_storage mSpiffs_storage;
  static bool sort(Configbase* a, Configbase* b) {
     return (a -> id < b -> id);
  }
public:
  Configlist() {

  };

  void setup()
  {

  }

  void loop()
  {

  }

  bool size() const
  {
    return mConfigArray.size();
  }

  std::vector<Configbase*> all()
  {
    std::sort (mConfigArray.begin(), mConfigArray.end(), sort);
    //How to return a read only vector?
    return mConfigArray;
  }

  bool load(Default_configbase* default_config = nullptr, bool deleteOldConfig = false, bool addInstances = true)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Configlist::load"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    if (deleteOldConfig)
      clear(true);

    std::vector <String> files = mSpiffs_config.list();
    if (files.size() == 0 && default_config != nullptr) {
      default_config -> addConfigs();
      if (deleteOldConfig)
        save();
      return true;
    }
    std::sort(files.begin(), files.end());
    byte id = 0;
    int configuration = 0;
    for (int i = 0; i < files.size(); i++) {
      //id and configuration are passed by reference
      //Load the shared id and configuration part of the config file
      mSpiffs_config.loadBase(files[i], id, configuration);
      //Get the instance
      Configbase* config = Configfactory::getConfigInstance(id, configuration);
      //Populate the values
      mSpiffs_config.load(config);
      if (addInstances) {
        mConfigArray.push_back(config);
      }
    //else
      //mSpiffs_storage.read(files[i], new Logger_writer());
    }
    return true;
  }

  bool add(Configbase* config, bool save = false)
  {
    if (config != nullptr)
    {
      if (get(config -> id) == nullptr)
      {
        mConfigArray.push_back(config);
        if (save)
          mSpiffs_config.save(config);
        return true;
      }
    }
    return false;
  }

  Configbase* create(byte id, int configuration)
  {
    Configbase* baseCfg = get(id, configuration);
    if (baseCfg == nullptr)
    {
      baseCfg = Configfactory::getConfigInstance(id, configuration);
      mConfigArray.push_back(baseCfg);
    }
    return baseCfg;
  }

  bool remove(byte id, bool save = false)
  {
    int toDelete = -1;
    Configbase* config = nullptr;
    for (int i=0; i<mConfigArray.size(); i++)
    {
      if (mConfigArray[i] -> id == id)
      {
        config = mConfigArray[i];
        toDelete = i;
        break;
      }
    }
    if (toDelete > -1)
    {
      mConfigArray.erase(mConfigArray.begin() + toDelete);
      if (config != nullptr)
        delete config;
      if (save)
        return mSpiffs_config.remove(id);
      return true;
    }
    return false;
  }

  void clear(bool deleteConfigs = false)
  {
    while (mConfigArray.size() > 0)
    {
      Configbase* config = nullptr;
      config = mConfigArray.back();
      mConfigArray.pop_back();
      //config = nullptr;
      delete config;
    }
    mConfigArray.clear();
    mConfigArray.shrink_to_fit();
    if (deleteConfigs)
      mSpiffs_config.removeAll();
  }

  void save()
  {
    for (int i=0; i<mConfigArray.size(); i++)
      mSpiffs_config.save(mConfigArray[i]);
  }

  void swap(byte id1, byte id2)
  {
    Configbase* config1 = get(id1);
    Configbase* config2 = get(id2);
    std::swap(config1, config2);
    save();
  }

  Configbase* get(byte id, int configuration = Configurations::NONE)
  {
    for (int i=0; i<mConfigArray.size(); i++)
    {
      if (mConfigArray[i] -> id == id && (mConfigArray[i] -> configuration == configuration || configuration == Configurations::NONE))
        return mConfigArray[i];
    }
    return nullptr;
  }

  bool save(byte id)
  {
    for (int i=0; i<mConfigArray.size(); i++)
    {
      if (mConfigArray[i] -> id == id)
        return mSpiffs_config.save(mConfigArray[i]);
    }
  }
};
#endif
