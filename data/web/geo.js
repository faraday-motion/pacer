
var enabled = false;
var powerAngle = 90;

/*
window.ondeviceorientation = function(event) {
  if (connected)
  {
    powerAngle = event.alpha;
  }
  var x = event.alpha;
  var y = event.beta;
  var z = event.gamma;

  var obj = setFromEulerAngles(x, y, z);

  //document.getElementById("lblangle1").innerText = "";
  document.getElementById("lblangle1").innerText = obj.x;
  document.getElementById("lblangle2").innerText = obj.y;
  document.getElementById("lblangle3").innerText = obj.z;
}
*/

//window.addEventListener("deviceorientation", handleOrientation, true);

//function handleOrientation(event) {
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

    //document.getElementById("lblangle1").innerText = betaR;
    //document.getElementById("lblangle2").innerText = gammaR;
    document.getElementById("lblangle3").innerText = Math.floor(spin);
}

document.getElementById("lblangle1").innerText = "";

/*
https://www.w3schools.com/html/html5_geolocation.asp
var x = document.getElementById("demo");
function getLocation() {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(showPosition);
    } else {
        x.innerHTML = "Geolocation is not supported by this browser.";
    }
}
function showPosition(position) {
    x.innerHTML = "Latitude: " + position.coords.latitude +
    "<br>Longitude: " + position.coords.longitude;
}
function showError(error) {
    switch(error.code) {
        case error.PERMISSION_DENIED:
            x.innerHTML = "User denied the request for Geolocation."
            break;
        case error.POSITION_UNAVAILABLE:
            x.innerHTML = "Location information is unavailable."
            break;
        case error.TIMEOUT:
            x.innerHTML = "The request to get user location timed out."
            break;
        case error.UNKNOWN_ERROR:
            x.innerHTML = "An unknown error occurred."
            break;
    }
}
function showPosition(position) {
    var latlon = position.coords.latitude + "," + position.coords.longitude;

    var img_url = "https://maps.googleapis.com/maps/api/staticmap?center=
    "+latlon+"&zoom=14&size=400x300&sensor=false&key=YOUR_:KEY";

    document.getElementById("mapholder").innerHTML = "<img src='"+img_url+"'>";
}
*/
