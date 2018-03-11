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
  IStore * pIStore;
  IConfigStore * pIConfigstore;
  static bool sort(Configbase * a, Configbase * b) {
     return (a -> getId() < b -> getId());
  }
public:
  Configlist() {
    pIStore = new Spiffs_storage();
    pIConfigstore = new Spiffs_config(pIStore);
  };

  Configlist(IStore * iStore, IConfigStore * iConfigstore) {
    pIStore = iStore;
    pIConfigstore = iConfigstore;
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

  bool load(Default_configbase * default_config = nullptr, bool deleteOldConfig = false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Configlist::load"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    if (deleteOldConfig)
      clear(true);

    std::vector <String> files;
    pIConfigstore -> list(files);
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
      pIConfigstore -> load(files[i], id, configuration);
      //Get the instance
      Configbase* config = Configfactory::getConfigInstance(id, configuration);
      //Populate the values
      pIConfigstore -> load(config);
      mConfigArray.push_back(config);
    //else
      //mSpiffs_storage.read(files[i], new Logger_writer());
    }
    return true;
  }

  bool add(Configbase * config, bool store = false)
  {
    if (config != nullptr)
    {
      while (get(config -> getId()) != nullptr)
        config -> setId(config -> getId()+1);
      mConfigArray.push_back(config);
      if (store)
        pIConfigstore -> save(config);
      return true;
    }
    return false;
  }

  bool add(int configuration, bool store = false)
  {
    Configbase * config = create(configuration);
    if (store)
      pIConfigstore -> save(config);
    return true;
  }

  Configbase * create(byte id, int configuration)
  {
    Configbase * baseCfg = get(id, configuration);
    if (baseCfg == nullptr)
    {
      baseCfg = Configfactory::getConfigInstance(id, configuration);
      mConfigArray.push_back(baseCfg);
    }
    return baseCfg;
  }

  Configbase * create(int configuration)
  {
    byte id = 0;
    Configbase * config = Configfactory::getConfigInstance(id, configuration);
    while (get(config -> getId()) != nullptr)
      config -> setId(config -> getId()+1);
    mConfigArray.push_back(config);
    return config;
  }

  bool remove(byte id, bool save = false)
  {
    int toDelete = -1;
    Configbase * config = nullptr;
    for (int i=0; i<mConfigArray.size(); i++)
    {
      if (mConfigArray[i] -> getId() == id)
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
        pIConfigstore -> remove(id);
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
      delete config;
    }
    mConfigArray.clear();
    mConfigArray.shrink_to_fit();
    if (deleteConfigs)
      pIConfigstore -> remove();
  }

  void save()
  {
    for (int i=0; i<mConfigArray.size(); i++)
      pIConfigstore -> save(mConfigArray[i]);
  }

  void swap(byte id1, byte id2, bool store = false)
  {
    Configbase * config1 = get(id1);
    Configbase * config2 = get(id2);
    config1 -> setId(id2);
    config2 -> setId(id1);
    if (store)
      save();
  }

  Configbase* get(byte id, int configuration = Configurations::NONE)
  {
    for (int i=0; i<mConfigArray.size(); i++)
    {
      if (mConfigArray[i] -> getId() == id && (mConfigArray[i] -> configuration == configuration || configuration == Configurations::NONE))
        return mConfigArray[i];
    }
    return nullptr;
  }

  bool save(byte id)
  {
    for (int i=0; i<mConfigArray.size(); i++)
    {
      if (mConfigArray[i] -> getId() == id)
        pIConfigstore -> save(mConfigArray[i]);
    }
    return true;
  }
};
#endif
