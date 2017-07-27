<p align="center">
  <img src="http://www.faradaymotion.com/wp-content/uploads/2017/07/pace_logo.png" width="150">
</p>

# What is Pacer?
Pacer is a development platform that enables fast and cheap development of light electric vehicles.

Pacer bundles open source software with modular hardware componets.

Pacer was created to simplify the development of light electrict vehicles by providing out of the box trivial functionality like connectivity, controlling and state management of the vehicle. 

# DISCLAIMER
1. The current version of the Pacer is not production ready. DO NOT use it unless thoroughly tested. 
2. This is a very stripped version of the documentation. A proper documentation is being worked on right now.

# How it works?

<p align="center"> 
  <img src="http://forum.faradaymotion.com/assets/files/2017-07-16/13:15:330-faraday-pitch-deck-01.jpeg" width="100%">
</p>

<p align="center"> 
  <img src="http://forum.faradaymotion.com/assets/files/2017-07-16/13:15:330-faraday-pitch-deck-02.jpeg" width="100%">
</p>

# Getting Started

## Introduction 

Pacer is build on top of the [Arduino core for ESP8266](https://github.com/esp8266/Arduino#documentation) and is running on the NODEMCU ESP8266. 

[PlatformIO](http://platformio.org/) is the development environmnet that is managing all the project dependencies.

## Software Getting Started

1. Setup a PlatformIO development environment for ESP8266. ([Check this guide](https://github.com/esp8266/Arduino#using-platformio))

2. Upload the configuration files to the ESP8266 flash memory `pio run --target uploadfs` (run this command in the root of the project)

3. Build and Upload `pio run --target uploadfs` (run this command in the root of the project)

4. You have a running Pacer instance on the NODEMCU. 

## Hardware Getting Started 

### PIN Configurations



## Pacer Modules 

- Connections
  - Wi-Fi Access Point 
  - Websockets
  - NRF24
  
- Controllers
  - Phone Controller (Android (not public yet) , [iOS](https://itunes.apple.com/ag/app/faraday-motion-controller/id1075493675?mt=8))
  - Nunchuck Controller (source code comng soon) 
  - Accelerometer Controller ([MPU9150](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU9150))
  
- Motor Controller 
  - VESC
  - more to come..
  
- Configurartion 
  - Config Controller 
  - JSON config file stored on the flash.

# Community 

There is a [forum thread](http://forum.faradaymotion.com/d/19-what-is-pacer/2) dedicated to Pacer where you can get quick answers to your questions.

# Support Us

You can support the Pacer project buy purchasing hardware required for running Pacer and other products from the Faraday Motion Web Shop. We highly appreciate your support. 
