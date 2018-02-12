var config = 

{
  "modules" : {
    "radio" : 0,
    "webSocketServer" : 1,
    "webServer" : 1
  },

  "wifi" : {
    "ssid"     : "Freshfaraday",
    "port"     : 8899,
    "ip"       : [10, 10, 100, 254],
    "subnet"   : [255, 255, 255, 0],
    "channel"  : 11,
    "pass"     : "freshfaraday"
  },

  "websocket" : {
    "port" : 81
  },

  "controller" : {
    "defaultSmoothAlpha"          : 0.5 ,
    "defaultInputNeutral"         : 50  ,
    "defaultInputMinBrake"        : 48  ,
    "defaultInputMaxBrake"        : 0   ,
    "defaultInputMinAcceleration" : 52  ,
    "defaultInputMaxAcceleration" : 100
  },

  "currentControl" : {
    "defaultCurrentNeutral"         : 0   ,
    "defaultCurrentBrakeMin"        : 0   ,
    "defaultCurrentBrakeMax"        : 60  ,
    "defaultCurrentAccelerationMin" : 0.25,
    "defaultCurrentAccelerationMax" : 30
  },

  "motorCount" : 2,

  "authorizedControllers" : [1, 2, 3, 4, 5],

  "registeredControllers" : [
    {
      "id" : "ACCE1",
      "type" : 3,
      "priority" : 1,
      "enabled" : 0,
      "constraints" : {
        "brake" : 200,
        "accel" : 650
      }
    },
    {
      "id" : "JOYS1",
      "type" : 4,
      "priority" : 1,
      "enabled" : 0,
      "constraints" : {
        "brake" : 190,
        "accel" : 840
      }
    }
  ]
}
