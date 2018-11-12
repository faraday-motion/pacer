
var tiltModule = (function () {
  // private
  var tiltAngle = 90;
  var joypower;
  var joydirection;
  var isEnabled = false;
  var driveEnabled = false;
  var onLabel = "ON";
  var offLabel = "OFF";

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

  function mapValue(x, in_min, in_max, out_min, out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  function power()
  {
    //100 - 170
    if (!isEnabled || !driveEnabled)
      return 0;
    var power = parseInt(mapValue(tiltAngle, 100, 170, 0, 100));
    if (power > 0)
     return limit(power);
    return 0;
  }

  function brake()
  {
    //10-80
    if (!isEnabled || !driveEnabled)
      return 0;
    var brake = parseInt(mapValue(tiltAngle, 80, 10, 0, 100));
    if (brake > 0)
     return limit(brake);
    return 0;
  }

  function left()
  {
    if (!isEnabled || !driveEnabled)
      return 0;
    return 0;
  }

  function right()
  {
    if (!isEnabled || !driveEnabled)
      return 0;
    return 0;
  }

  function regionId()
  {
    return "region_tilt";
  }

  (function initialize() {
    window.addEventListener("devicemotion", function(event) {
      //document.getElementById("lblangle3").innerText = "acceleration x:" + event.acceleration.x + " y:" + event.acceleration.y + " z:" + + event.acceleration.z;
      //document.getElementById("lblangle4").innerText = "accelerationIncludingGravity x:" + event.accelerationIncludingGravity.x + " y:" + event.accelerationIncludingGravity.y + " z:" + + event.accelerationIncludingGravity.z;
    });

    window.ondeviceorientation = function(event) {
        // those angles are in degrees
        var alpha = event.alpha;
        var beta = event.beta;
        var gamma = event.gamma;

        // JS math works in radians
        var betaR = beta / 180 * Math.PI;
        var gammaR = gamma / 180 * Math.PI;
        var spinR = Math.atan2(Math.cos(betaR) * Math.sin(gammaR), Math.sin(betaR));

        // convert back to degrees
        var spin = spinR * 180 / Math.PI;
        tiltAngle = Math.abs(Math.floor(spin));
    }

    Pressure.set('#btnenabletilt', {
      start: function(event){
        // this is called on force start
        if(isEnabled)
        {
          $("#btnenabletilt").removeClass("btn-danger").addClass("btn-success");
          $("#btnenabletilt").text(onLabel);
          driveEnabled = true;
        }
      },
      end: function(){
        // this is called on force end
        $("#btnenabletilt").removeClass("btn-success").addClass("btn-danger");
        $("#btnenabletilt").text(offLabel);
        driveEnabled = false;
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
      },
      unsupported: function(){
        // NOTE: this is only called if the polyfill option is disabled!
        // this is called once there is a touch on the element and the device or browser does not support Force or 3D touch
      }
    }, {polyfillSpeedUp: 500, polyfillSpeedDown: 500});

    setInterval(
      function(){
        document.getElementById("lblangle1").innerText = tiltAngle;
    }, 100);
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
