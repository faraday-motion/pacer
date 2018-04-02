var pacerModule = (function () {
  // private
  var enabled = false;
  var ledEnabled = false;
  var currentCommand = null;
  var triState = 0;
  var sensors = null;
  var activeController = null;

  function setEnabled() {
    if (websocketModule.connected())
    {
      enabled = true;
      $("#btnenable").css("background-color", "green");
      $("#btnenable").text("Enabled!");
    }
  }

  function setDisabled() {
      enabled = false;
      $("#btnenable").css("background-color", "red");
      $("#btnenable").text("Disabled!");
  }

  function toggleEnabled()
  {
    if (websocketModule.connected())
    {
      if (!enabled)
        setEnabled();
      else
        setDisabled();
    }
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
        $("#btnled").css("background-color", "red");
      }
      else
      {
        commandsModule.ledOn();
        $("#btnled").css("background-color", "green");
      }
    }
  }

  function send()
  {
    if (websocketModule.connected())
    {
      if (enabled)
      {
        if (triState == 0)
        {
          if (activeController)
          {
            console.log("brake:" + activeController.brake());
            console.log("power:" + activeController.power());
            if (activeController.brake() > 0)
              commandsModule.brake(activeController.brake());
            else if (activeController.power() > 0)
              commandsModule.power(activeController.power());
            else
              commandsModule.powerNeutral();
          }
        }
        else if (triState == 1)
        {
          if (activeController)
          {
            console.log("left:" + activeController.left());
            console.log("right:" + activeController.right());
            if (activeController.left() > 0)
              commandsModule.left(activeController.left());
            else if (activeController.right() > 0)
              commandsModule.right(activeController.right());
            else
              commandsModule.directionNeutral();
          }
        }
      }
      else
      {
        if (activeController)
        {
          if (triState == 0)
            commandsModule.powerNeutral();
          else if (triState == 1)
            commandsModule.directionNeutral();
        }
      }

      triState ++;
      if (triState == 3)
        triState = 0;

      if (currentCommand == null)
        currentCommand = commandsModule.getNext();
      if (currentCommand)
        websocketModule.send(currentCommand.command, currentCommand.value);
    }
  }

  function connectChange(connected)
  {
    if (connected)
    {
      commandsModule.clear();
      $("#lblstate").text("Connected");
    }
    else
    {
      $("#lblstate").text("Disconnected");
      setDisabled();
    }
  }

//TODO if we havent gotten any response for a short time, consider the connection as being closed.
  function messageRecieved(msg)
  {
    if (msg)
    {
      consoleModule.console(msg);
      if (msg.sensors)
      {
        sensors = msg.sensors;
      }
      if (msg.command && currentCommand)
      {
        if (msg.command == currentCommand.command)
          currentCommand = null;
        if (msg.command == 14)
          ledEnabled = true;
        if (msg.command == 15)
          ledEnabled = false;
      }
    }
  }

  function updateSensors()
  {
    if (sensors)
    {
      var s = "";
      $.each(sensors, function(key, value) {
        s += key + ": " + value + "<br>";
      });
      $("#lblsensors").html(s);
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

  (function initialize() {
    //activeController = joystickModule;
    //activeController = tiltModule;
    setInterval(
      function(){
        send();
    }, 25);

    setInterval(
      function(){
        updateSensors();
    }, 100);

    $(document).ready(function(){
      $("#btnenable").css("background-color", "red");
      $("#btnled").css("background-color", "green");

      $("#btnenable").click(function(){
          toggleEnabled();
      });
      $("#btnled").click(function(){
          toggleLedEnabled();
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
