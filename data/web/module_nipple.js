var nippleModule = (function () {
  // private
  var manager = null;
  var isEnabled = false;
  var joystickValues = {x:0,y:0};

  function setEnabled(enabled)
  {
    isEnabled = enabled;
  }

  function mapValue(x, in_min, in_max, out_min, out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
    var power = parseInt(joystickValues.y);
    if (power > 0)
     return limit(power);
    return 0;
  }

  function brake()
  {
    if (!isEnabled)
      return 0;
    var brake = parseInt(joystickValues.y);
    if (brake < 0)
     return limit(-brake);
    return 0;
  }

  function left()
  {
    if (!isEnabled)
      return 0;
    var left = parseInt(joystickValues.x);
    if (left < 0)
     return limit(-left);
    return 0;
  }

  function right()
  {
    if (!isEnabled)
      return 0;
    var right = parseInt(joystickValues.x);
    if (right > 0)
     return limit(right);
    return 0;
  }

  function regionId()
  {
    return "region_nipple";
  }

  (function initialize() {
    var options = {
    zone: document.getElementById('nipplejoystick'),                  // active zone
    color: '#000000',
    //size: Integer,
    //threshold: Float,               // before triggering a directional event
    //fadeTime: Integer,              // transition time
    //multitouch: false,
    //maxNumberOfNipples: 1,     // when multitouch, what is too many?
    //dataOnly: Boolean,              // no dom element whatsoever
    //position: Object,               // preset position for 'static' mode
    mode: 'dynamic'//,                   // 'dynamic', 'static' or 'semi'
    //restJoystick: Boolean,
    //restOpacity: Number,            // opacity when not 'dynamic' and rested
    //catchDistance: Number           // distance to recycle previous joystick in
                                    // 'semi' mode
    };
    manager = nipplejs.create(options);
    manager.on('added', function (evt, nipple) {
      nipple.on('start move end dir plain', function (evt, obj) {
        if (evt.type == 'move')
        {
          if (isEnabled)
          {
            var xReading = parseInt(obj.distance * Math.cos(obj.angle.radian));
            var yReading = parseInt(obj.distance * Math.sin(obj.angle.radian));
            joystickValues.x = mapValue(xReading, -50, 50, -100, 100);
            joystickValues.y = mapValue(yReading, -50, 50, -100, 100);
          }
          else
          {
            joystickValues.x = 0;
            joystickValues.y = 0;
          }
        }
      });
    }).on('removed', function (evt, nipple) {
      nipple.off('start move end dir plain');
      joystickValues.x = 0;
      joystickValues.y = 0;
    });
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
