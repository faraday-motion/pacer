<p align="center">
  <img src="http://www.faradaymotion.com/wp-content/uploads/2017/07/pace_logo.png" width="150">
</p>

# What is Pacer?
Pacer is a development platform that enables fast and cheap development of light electric vehicles.

Pacer bundles open source software with modular hardware components.

Pacer was created to simplify the development of light electric vehicles by providing out of the box trivial functionality like connectivity, controlling and state management of the vehicle. 

# DISCLAIMER
1. The current version of Pacer is not production ready. DO NOT use it unless thoroughly tested. 
2. This is a very stripped version of the documentation. A proper documentation is being worked on right now. If you need help shoot us a question on our [forum](http://forum.faradaymotion.com).

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

[PlatformIO](http://platformio.org/) is the development environment that is managing all the project dependencies for Pacer.

## Pacer Related Repositories
[Pacer Controller](https://github.com/faraday-motion/pacer-web-app) - A RF controller for Pacer powered vehicles

[Pacer Web App](https://github.com/faraday-motion/pacer-controller) - A web app hosted on Pacer powered vehicles that allows configuration and has a setup wizzard.

## Software Getting Started

1. Set up a PlatformIO development environment for ESP8266. ([Check this guide](https://github.com/esp8266/Arduino#using-platformio))

2. Upload the configuration files to the ESP8266 flash memory `pio run --target uploadfs` (run this command in the root of the project)

3. Build and Upload `pio run --target upload` (run this command in the root of the project)
   Note: You might want to look through the config file in `data/` directory and change relevant configurations.

4. You have a running Pacer instance on the NODEMCU. 

## Hardware Getting Started 

### Steps
---------
In order to have a running vehicle in its simplest form you require the following components: 
- BLDC Motor 
- Motor Controller (for now we only support VESC)
- Battery for power supply (max: 12S configuration)
- NodeMCU
- nRF24 module (optional)
- MPU9150 module (optional)
- Wired Controller (optional)

Steps:

1. Connect the NodeMCU, nRF24 (optional), MPU9150 (optional), Wired Controller (optional) together according to the PIN Configuration section below.

2. Make sure you have uploaded the configuration files and the binary file to the NodeMCU. (see Software Getting Started.)

3. You are ready to go. 


### PIN Configurations
----------------------

#### 1. VESC Motor Controller


| NodeMCU  |     VESC      |
|----------|:-------------:|
| TX       | RX            |
| RX       | TX            |
| GND      | GND           |
| VIN      | 5v            |

#### 2. nRF24 Connections

![nRF24](http://www.faradaymotion.com/wp-content/uploads/2017/07/Screen-Shot-2017-07-27-at-12.09.51-PM.png)

![nRF24 Pins](http://www.faradaymotion.com/wp-content/uploads/2017/07/Screen-Shot-2017-07-27-at-12.23.36-PM.png)

#### 3. MPU9150

| NodeMCU               |     MPU9150   |
|-----------------------|:-------------:|
| D1(SCL) (GPIO 5)      | SCL           |
| D2(SDA) (GPIO 4)      | SDA           |
| GND                   | GND           |
| 3.3v                  | VCC           |


#### 4. Wired Controller

| NodeMCU               | Wired Joystick   |
|-----------------------|:----------------:|
| A0                    | X-Axis Pin       |
| GND                   | GND              |
| 3.3v                  | +5v              |

Note: This might differ depending on the joystick you are using. Unfortunately the NodeMcu only has one analog pin. 


## Pacer Modules 

- Connections
  - Wi-Fi Access Point 
  - Websockets
  - NRF24
  
- Controllers
  - Phone Controller (Android (not public yet) , [iOS](https://itunes.apple.com/ag/app/faraday-motion-controller/id1075493675?mt=8))
  - Nunchuck Controller (source code comng soon) 
  - Wired Joystick Controller
  - Accelerometer Controller ([MPU9150](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU9150))
  
- Motor Controller 
  - VESC
  - more to come..
  
- Configuration 
  - Config Controller 
  - JSON config file stored on the flash.

# Community 

There is a [forum thread](http://forum.faradaymotion.com/d/19-what-is-pacer/2) dedicated to Pacer where you can get quick answers to your questions.

# Support Us

You can support the Pacer project by purchasing hardware required for running Pacer and other products from the [Faraday Motion Web Shop](http://shop.faradaymotion.com). We highly appreciate your support. 

# Contributing

There is a long road of development until we get Pacer to be the go to solution. Any contributions are highly valued. 

For minor fixes of code and documentation, go ahead and submit a pull request.

Feature branches with lots of small commits (especially titled "oops", "fix typo", "forgot to add file", etc.) should be squashed before opening a pull request. At the same time, please refrain from putting multiple unrelated changes into a single pull request.

# Authors 
Constantin Colac and Sune Pedersen
