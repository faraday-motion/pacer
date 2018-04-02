var joystickModule = (function () {
  // private
  var joypower;
  var joydirection;

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
    var power = joypower.GetY();
    if (power > 0)
     return limit(power);
    return 0;
  }

  function brake()
  {
    var brake = joypower.GetY();
    if (brake < 0)
     return limit(-brake);
    return 0;
  }

  function left()
  {
    var left = joydirection.GetX();
    if (left < 0)
     return limit(-left);
    return 0;
  }

  function right()
  {
    var right = joydirection.GetX();
    if (right > 0)
     return limit(right);
    return 0;
  }

  (function initialize() {
    var parameters = {};
    parameters.internalFillColor = "#777777";
    parameters.internalLineWidth = 4;
    parameters.internalStrokeColor = "#222222";
    parameters.externalLineWidth = 2;
    parameters.externalStrokeColor = "#CCCCCC";
    joypower = new JoyStick('joypower', parameters);
    joydirection = new JoyStick('joydirection', parameters);
  })();

  return {
    // public
    power : power,
    brake : brake,
    left : left,
    right : right
  };
}());
