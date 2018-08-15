var websocketModule = (function (parameters) {
  // private
  parameters = parameters || {};
  var host = (parameters.host === undefined ? "ws://10.10.100.254:81" : parameters.host);
  var connectTimeout = (parameters.connectTimeout === undefined ? 3000 : parameters.connectTimeout);;
  var disconnectTimeout = (parameters.disconnectTimeout === undefined ? 3000 : parameters.disconnectTimeout);;

  var isConnected = false;
  var socket = null;
  var autoDisconnect;
  var id = 0;
  var lastMillis = 0;

  function getMillisDiff()
  {
    var d = new Date();
    var t = d.getTime();
    if (lastMillis == 0)
      lastMillis = t;
    var ret = t - lastMillis;
    lastMillis = t;
    return ret;
  }

  function getCurrentTime() {
    var d = new Date();
    var yyyy = d.getFullYear();
    var mm = d.getMonth() < 9 ? "0" + (d.getMonth() + 1) : (d.getMonth() + 1); // getMonth() is zero-based
    var dd  = d.getDate() < 10 ? "0" + d.getDate() : d.getDate();
    var hh = d.getHours() < 10 ? "0" + d.getHours() : d.getHours();
    var min = d.getMinutes() < 10 ? "0" + d.getMinutes() : d.getMinutes();
    var ss = d.getSeconds() < 10 ? "0" + d.getSeconds() : d.getSeconds();
    return "".concat(hh).concat(min).concat(ss);
  //  return "".concat(yyyy).concat(mm).concat(dd).concat(hh).concat(min).concat(ss);
  }

  function connect(connectCallback, messageRecieved){
    function triggerCallback()
    {
      if (connectCallback)
        connectCallback(isConnected);
    }

    try{
      if (socket)
      {
        allowSend = true;
        socket.close();
        socket = null;
      }
      socket = new WebSocket(host);
          console.log('WebSocket Socket Status: ' + socket.readyState);

          socket.onopen = function(){
            isConnected = true;
            console.log('onopen Socket Status: ' + socket.readyState + ' (open)');
            triggerCallback();
          }

          socket.onmessage = function(msg){
            if (autoDisconnect)
              clearTimeout(autoDisconnect);

            autoDisconnect = setTimeout(function(){
              if (isConnected)
              {
                isConnected = false;
                triggerCallback();
              }
            }, disconnectTimeout);

            if (msg.data.length) {
              var obj = JSON.parse(msg.data);
              if (obj.msg == "ping")
                sendRaw("{\"ping\":\"pong\"}");
              else
                messageRecieved(obj);
              console.log(getCurrentTime() + " " + getMillisDiff() + " Response::", msg.data);
            } else {
              console.log(getCurrentTime() + " " + getMillisDiff() + " Response is empty.");
            }
          }

          socket.onclose = function(){
            isConnected = false;
            console.log('onclose Socket Status: ' + socket.readyState + ' (Closed)');
            triggerCallback();
          }

          window.onbeforeunload = function(event) {
            socket.close();
          };
      }
      catch(exception){
        isConnected = false;
        socket = null;
        triggerCallback();
        console.log('exception: ' + exception);
      }
  }

  function sendRaw(msg)
  {
    if (socket && socket.readyState == 1)
      socket.send(msg);
  }

  function initialize(connectCallback, messageRecieved)
  {
    connect(connectCallback, messageRecieved);
    setInterval(
      function(){
        if (!isConnected)
          connect(connectCallback, messageRecieved);
    }, connectTimeout);
  }

  function connected()
  {
    return isConnected;
  }

  return {
    // public
    initialize : initialize,
    connected : connected,
    sendRaw : sendRaw
  };
}());
