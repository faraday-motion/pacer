var commandsModule = (function () {
  // private
  var queue = [];
  var queueMax = 10;

  var MODULE_ON = 0;
  var MODULE_OFF = 1;
  var DISABLE_CONTROLLERS = 2;
  var ENABLE_CONTROLLER = 3;
  var DRIVE_POWER = 4;
  var DRIVE_BRAKE = 5;
  var TURN_LEFT = 6;
  var TURN_RIGHT = 7;
  var DRIVE_MODE_0 = 8;
  var DRIVE_MODE_20 = 9;
  var DRIVE_MODE_40 = 10;
  var DRIVE_MODE_60 = 11;
  var DRIVE_MODE_80 = 12;
  var DRIVE_MODE_100 = 13;
  var LED_ENABLED = 14;
  var LED_DISABLED = 15;

  function addCommand(cmd, val)
  {
    while (queue.length > queueMax)
      queue.shift();
    //If we get a new value on the same command before its being sent, overwrite instead of add
    var hasValue = false;
    for (i = 0; i < queue.length; i++) {
      if (queue[i].command == cmd)
      {
        hasValue = true;
        queue[i].value = val;
        break;
      }
    };
    if (hasValue == false)
    {
      var obj = {command:cmd, value:val};
      queue.push(obj);
    }
  }

  function getNext()
  {
    if (queue.length > 0)
    {
      var obj = queue.shift();
      return obj;
    }
    return null;
  }

  function clear()
  {
    while (queue.length > 0)
    {
      queue.shift();
    }
  }

  function power(power)
  {
    addCommand(DRIVE_POWER, power);
  }

  function brake(brake)
  {
    addCommand(DRIVE_BRAKE, brake);
  }

  function powerNeutral()
  {
    addCommand(DRIVE_BRAKE, 0);
  }

  function left(left)
  {
    addCommand(TURN_LEFT, left);
  }

  function right(right)
  {
    addCommand(TURN_RIGHT, right);
  }

  function directionNeutral()
  {
    addCommand(TURN_RIGHT, 0);
  }

  function ledOn()
  {
    addCommand(LED_ENABLED, 1);
  }

  function ledOff()
  {
    addCommand(LED_DISABLED, 1);
  }

  return {
    // public
    clear : clear,
    getNext : getNext,
    power : power,
    brake : brake,
    powerNeutral : powerNeutral,
    left : left,
    right : right,
    directionNeutral : directionNeutral,
    ledOn : ledOn,
    ledOff : ledOff,
    addCommand : addCommand
  };
}());
