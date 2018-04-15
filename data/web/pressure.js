var pressureModule = (function () {
  // private
    var powerValue = 0;
    var brakeValue = 0;
    var powerLabel = "Accelerate";
    var brakeLabel = "Brake";
    var isEnabled = false;

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
      var power = parseInt(mapValue(powerValue, 0, 1, 0, 100));
      if (power > 0)
       return limit(power);
      return 0;
    }

    function brake()
    {
      if (!isEnabled)
       return 0;
      var brake = parseInt(mapValue(brakeValue, 0, 1, 0, 100));
      if (brake > 0)
       return limit(brake);
      return 0;
    }

    function left()
    {
      if (!isEnabled)
        return 0;
      return 0;
    }

    function right()
    {
      if (!isEnabled)
        return 0;
      return 0;
    }


  function setupButtons()
  {
    Pressure.set('#btnpressureaccel', {
      start: function(event){
        // this is called on force start
        powerValue = 0;
      },
      end: function(){
        // this is called on force end
        powerValue = 0;
      },
      startDeepPress: function(event){
        // this is called on "force click" / "deep press", aka once the force is greater than 0.5
      },
      endDeepPress: function(){
        // this is called when the "force click" / "deep press" end
      },
      change: function(force, event){
        // this is called every time there is a change in pressure
        // 'force' is a value ranging from 0 to 1
        if (isEnabled)
          powerValue = force;
      },
      unsupported: function(){
        // NOTE: this is only called if the polyfill option is disabled!
        // this is called once there is a touch on the element and the device or browser does not support Force or 3D touch
      }
    }, {polyfillSpeedUp: 3000, polyfillSpeedDown: 1000});

    Pressure.set('#btnpressurebrake', {
      start: function(event){
        // this is called on force start
        brakeValue = 0;
      },
      end: function(){
        // this is called on force end
        brakeValue = 0;
      },
      startDeepPress: function(event){
        // this is called on "force click" / "deep press", aka once the force is greater than 0.5
      },
      endDeepPress: function(){
        // this is called when the "force click" / "deep press" end
      },
      change: function(force, event){
        // this is called every time there is a change in pressure
        // 'force' is a value ranging from 0 to 1
        if (isEnabled)
          brakeValue = force;
      },
      unsupported: function(){
        // NOTE: this is only called if the polyfill option is disabled!
        // this is called once there is a touch on the element and the device or browser does not support Force or 3D touch
      }
    }, {polyfillSpeedUp: 1000, polyfillSpeedDown: 250});
  }

  function regionId()
  {
    return "region_pressure";
  }

  (function initialize() {
    setupButtons();

    setInterval(
      function(){
        if (isEnabled)
        {
          $("#btnpressureaccel").text(powerLabel + " " + power());
          $("#btnpressurebrake").text(brakeLabel + " " + brake());
        }
    }, 50);
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
