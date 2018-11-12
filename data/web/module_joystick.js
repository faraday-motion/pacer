var joystickModule = (function () {
  // private
  var joypower;
  var joydirection;
  var isEnabled = false;

  function setEnabled(enabled)
  {
    isEnabled = enabled;
  }

  function limit(val)
  {
    if (val > 100)
      return 100;
    if (val < 0)
      return 0;
    return val;
  }

  function power()
  {
    if (!isEnabled)
      return 0;
    var power = parseInt(joypower.GetY());
    if (power > 0)
     return limit(power);
    return 0;
  }

  function brake()
  {
    if (!isEnabled)
      return 0;
    var brake = parseInt(joypower.GetY());
    if (brake < 0)
     return limit(-brake);
    return 0;
  }

  function left()
  {
    if (!isEnabled)
      return 0;
    var left = parseInt(joypower.GetX());
    if (left < 0)
     return limit(-left);
    return 0;
  }

  function right()
  {
    if (!isEnabled)
      return 0;
    var right = parseInt(joypower.GetX());
    if (right > 0)
     return limit(right);
    return 0;
  }

  function regionId()
  {
    return "region_joystick";
  }

  (function initialize() {
    var parameters = {};
    parameters.internalFillColor = "#777777";
    parameters.internalLineWidth = 4;
    parameters.internalStrokeColor = "#222222";
    parameters.externalLineWidth = 2;
    parameters.externalStrokeColor = "#CCCCCC";
    parameters.title = "joyjoystick";
    joypower = new JoyStick('joyjoystick', parameters);
  })();

  return {
    // public
    power : power,
    brake : brake,
    left : left,
    right : right,
    regionId : regionId,
    setEnabled : setEnabled
  };
}());
