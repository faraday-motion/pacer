var newConfig = config; // make a copy of the default config. 

function init() 
{ 
  console.log("Init");
  fillDefaultConfig();
}

function handleResponse(payload)
{
  console.log(payload);
}

function saveConfiguration() 
{
  validated = false;
  validated = setWifiConfig();
  validated = setModulesConfig();
  validated = setWebsocketConfig();
  validated = setFMVConfig();
  validated = setControllerConfig();

  if (!validated)  {
    console.log("validation failed");
  } else { 
    let payload = "8007:" + JSON.stringify(newConfig, undefined, 2);
    socket.send(payload);
  }
}

function fillDefaultConfig()
{
  getWifiConfig();
  getModulesConfig();
  getWebsocketsConfig();
  getFMVConfig();
  getControllerConfig();
}

function setControllerConfig() {
  controllers = [];
  if (document.getElementById("wifiCtrl").checked)
    controllers.push(1);
  if (document.getElementById("radiCtrl").checked)
    controllers.push(2);  
  if (document.getElementById("acceCtrl").checked)
    controllers.push(3);
  if (document.getElementById("wireCtrl").checked)
    controllers.push(4);

  if (controllers.length < 1) {
    alert("At least one type of controllers should be chosen.");
    return false;
  } 

  return true;
}

function getControllerConfig() {
  // Controllers 
  document.getElementById("wifiCtrl").checked = config.authorizedControllers.indexOf(1) != -1 ? true : false;
  document.getElementById("radiCtrl").checked = config.authorizedControllers.indexOf(2) != -1 ? true : false;
  document.getElementById("acceCtrl").checked = config.authorizedControllers.indexOf(3) != -1 ? true : false;
  document.getElementById("wireCtrl").checked = config.authorizedControllers.indexOf(4) != -1 ? true : false;
}

function getFMVConfig() { 
   // FMV Params Config
  if (config.motorCount == 1)
    document.getElementById("vesc1").checked = true;
  if (config.motorCount == 2)
    document.getElementById("vesc2").checked = true;
  if (config.motorCount == 4)
    document.getElementById("vesc4").checked = true;
}

function setFMVConfig() { 
  var motorCount; 
  if(document.getElementById("vesc1").checked)
    motorCount = 1;
  if(document.getElementById("vesc2").checked)
    motorCount = 2;
  if(document.getElementById("vesc4").checked)
    motorCount = 4;

  if (motorCount < 1 || motorCount == 3 || motorCount > 4) {
    alert("For now we support vehicles wiht 1, 2 or 4 VESCs");
    return false;
  } else { 
    newConfig.motorCount = motorCount;
  }
}

function getWebsocketsConfig(){ 
  //Websockets
  document.getElementById("websocketPort").value = config.websocket.port;
}

function setWebsocketConfig() {
  port = document.getElementById("websocketPort").value;
  if (port < 1 || port > 9999) {
    alert("Available port range is 1-9999 inclusive");
    return false;
  } else { 
    newConfig.websocket.port = port;
  }
}

function getModulesConfig() { 
  // Modules
  document.getElementById("websocketEnabled").checked = config.modules.webSocketServer;
  document.getElementById("webServerEnabled").checked = config.modules.webServer;
  document.getElementById("radio").checked = config.modules.radio;
}

function setModulesConfig() { 
  newConfig.modules.webSocketServer = document.getElementById("websocketEnabled").checked;
  newConfig.modules.webServer = document.getElementById("webServerEnabled").checked;
  newConfig.modules.radio = document.getElementById("radio").checked;
  return true;
}

function getWifiConfig() 
{
   // Wifi
  document.getElementById("wifiSsid").value      = config.wifi.ssid;
  document.getElementById("wifiPassword").value  = config.wifi.pass;
  document.getElementById("wifiIpAddress").value = config.wifi.ip[0] + "." + config.wifi.ip[1] + "." + config.wifi.ip[2] + "." + config.wifi.ip[3];
  document.getElementById("wifiSubnet").value    = config.wifi.subnet[0] + "." + config.wifi.subnet[1] + "." + config.wifi.subnet[2] + "." + config.wifi.subnet[3];
  document.getElementById("wifiChannel").value   = config.wifi.channel;
  document.getElementById("wifiPort").value      = config.wifi.port;
}

function setWifiConfig() 
{
  
  ssid = document.getElementById("wifiSsid").value;
  if (ssid.length < 6) {
    alert("Wifi SSID is too short");
    return false;
  } else {
    newConfig.wifi.ssid = ssid;
  }

  pass = document.getElementById("wifiPassword").value
  if (pass.length < 6) {
    alert("Wifi Password is too short");
    return false;
  } else {
    newConfig.wifi.pass = pass;
  }

  ip = document.getElementById("wifiIpAddress").value.split(".");
  if (ip.length != 4) {
    alert("Something is wrong with the submitted ip");
    return false;
  } else { 
    for (i = 0; i < 4; i++) {
      if (ip[i].toString().length < 1 || ip[i].toString().length > 3) {
        alert("Wrong format. Expected: 100.100.10.1");
        return false;
      } else {
        newConfig.wifi.ip[i] = ip[i]; 
      }
    }
  }

  subnet = document.getElementById("wifiSubnet").value.split(".");
  if (subnet.length != 4) {
    alert("Something is wrong with the submitted subnet");
    return false;
  } else { 
    for (i = 0; i < 4; i++) {
      if (subnet[i].toString().length < 1 || subnet[i].toString().length > 3) {
        alert("Wrong format. Expected: 100.100.10.1");
        return false;
      } else {
        newConfig.wifi.subnet[i] = subnet[i]; 
      }
    }
  }


  channel = document.getElementById("wifiChannel").value; 
  if (channel < 1 || channel > 14) {
    alert("Wifi Channel ragne is 1-14 inclusive.");
    return false;
  } else { 
    newConfig.wifi.channel = channel;
  }


  port = document.getElementById("wifiPort").value;
  if (port < 1 || port > 9999) {
    alert("Allowed port range is 1-9999 inclusive");
    return false;
  } else { 
    newConfig.wifi.port = port;
  }

  return true;
}

init();
