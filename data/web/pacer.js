
//https://blog.phonegap.com/cordova-screen-orientation-plugin-fabeba30a4c4

var pacerModule = (function () {
  // private
  var enabled = false;
  var ledEnabled = false;
  var sensors = null;
  var activeController = null;
  var tabModules = [tiltModule, joystickModule, nippleModule, pressureModule, graphModule, consoleModule];
  var activeTab = 2;
  var messageId = 0;
  var currentCommand = null;

  function setEnabled() {
    if (websocketModule.connected())
    {
      enabled = true;
      if (activeController)
        activeController.setEnabled(enabled);
      $("#btnenable").removeClass("btn-secondary").removeClass("btn-dark").addClass("btn-light");
      $("#btnenable").text("Enabled");
    }
  }

  function setDisabled() {
    enabled = false;
    if (activeController)
      activeController.setEnabled(enabled);
    if (websocketModule.connected())
    {
      $("#btnenable").removeClass("btn-secondary").removeClass("btn-light").addClass("btn-dark");
      $("#btnenable").text("Disabled");
    }
    else
    {
      $("#btnenable").removeClass("btn-dark").removeClass("btn-light").addClass("btn-secondary");
      $("#btnenable").text("Connecting...");
    }
  }

  function toggleEnabled()
  {
    if (!enabled)
      setEnabled();
    else
      setDisabled();
  }

  function toggleLedEnabled()
  {
    if (websocketModule.connected())
    {
      if (ledEnabled)
      {
        commandsModule.ledOff();
        $("#btnled").removeClass("btn-success").addClass("btn-danger");
      }
      else
      {
        commandsModule.ledOn();
        $("#btnled").removeClass("btn-danger").addClass("btn-success");
      }
    }
  }

  function getControllerInput()
  {
    if (enabled && activeController)
    {
        if (activeController.brake() > 0)
          commandsModule.brake(activeController.brake());
        else if (activeController.power() > 0)
          commandsModule.power(activeController.power());
        else
          commandsModule.powerNeutral();

        if (activeController.left() > 0)
          commandsModule.left(activeController.left());
        else if (activeController.right() > 0)
          commandsModule.right(activeController.right());
        else
          commandsModule.directionNeutral();
    }
  }

  function send()
  {
    if (websocketModule.connected())
    {
      if (currentCommand == null)
      {
        currentCommand = commandsModule.getNext();
        if (currentCommand != null)
        {
          currentCommand.id = messageId;
          websocketModule.sendRaw("{\"id\":" + JSON.parse(currentCommand.id) + ",\"command\":" + JSON.parse(currentCommand.command) + ",\"value\":" + JSON.parse(currentCommand.value) + "}");
        }
      }
    }
  }

  function messageRecieved(msg)
  {
    if (msg)
    {
      if (currentCommand && msg.id && msg.id == currentCommand.id)
      {
        messageId ++;
        currentCommand = null;
      }
      consoleModule.console(msg);
      if (msg.sensors)
        sensors = msg.sensors;
      if (msg.command == 14)
        ledEnabled = true;
      if (msg.command == 15)
        ledEnabled = false;
    }
  }

  function connectChange(connected)
  {
    if (connected)
    {
      commandsModule.clear();
      $("#btnenable").removeClass("btn-secondary").addClass("btn-dark");
      $("#btnenable").text("Disabled");
    }
    else
    {
      $("#btnenable").removeClass("btn-secondary").addClass("btn-secondary");
      $("#btnenable").text("Connecting...");
      setDisabled();
    }
  }

  function updateSensors()
  {
    if (sensors)
    {
      graphModule.setSensors(sensors);
      $("#lblsensors").html(JSON.stringify(sensors, undefined, 4));
    }
  }

  function getSensors()
  {
    return sensors;
  }

  function setController(controller)
  {
    commandsModule.clear();
    activeController = controller;
  }

  function showActiveTab()
  {
    for (i = 0; i < tabModules.length; i++) {
      if (tabModules[i])
      {
        if (activeTab == i)
        {
          $("#" + tabModules[i].regionId()).show();
          setController(tabModules[i]);
        }
        else
        {
          $("#" + tabModules[i].regionId()).hide();
          tabModules[i].setEnabled(false);
        }
      }
    };
  }

  (function initialize() {
    showActiveTab();

    setInterval(
      function(){
        getControllerInput();
    }, 50);

    setInterval(
      function(){
        send();
    }, 25);

    setInterval(
      function(){
        updateSensors();
    }, 100);

    $(document).ready(function(){
      $(".ui-loader").hide();
      $("#btnled").removeClass("btn-danger").addClass("btn-danger");

      $("#btnenable").click(function(evt){
          toggleEnabled();
      });
      $("#btnled").click(function(){
          toggleLedEnabled();
      });

      $("body").on("swiperight", function(){
        if (!enabled && activeTab > 0)
        {
          activeTab--;
          showActiveTab();
        }
      });
      $("body").on("swipeleft", function(){
        if (!enabled && activeTab < tabModules.length -1)
        {
          activeTab++;
          showActiveTab();
        }
      });
    });
  })();

  return {
    // public
    connectChange : connectChange,
    messageRecieved : messageRecieved,
    getSensors : getSensors,
    setController : setController
  };
}());
