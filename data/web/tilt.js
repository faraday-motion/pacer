
var tiltModule = (function () {
  // private
  var tiltAngle = 90;
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

  function mapValue(x, in_min, in_max, out_min, out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  function power()
  {
    //100 - 170
    var power = mapValue(tiltAngle, 100, 170, 0, 100);
    if (power > 0)
     return limit(power);
    return 0;
  }

  function brake()
  {
    //10-80
    var brake = mapValue(tiltAngle, 80, 10, 0, 100);
    if (brake > 0)
     return limit(brake);
    return 0;
  }

  function left()
  {
    return 0;
  }

  function right()
  {
    return 0;
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
        document.getElementById("lblangle1").innerText = tiltAngle;
        document.getElementById("lblangle2").innerText = "alpha:" + alpha + " beta:" + beta + " gamma:" + gamma;
    }
  })();

  return {
    // public
    power : power,
    brake : brake,
    left : left,
    right : right
  };
}());
