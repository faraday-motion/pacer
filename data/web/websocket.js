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
              console.log("Response::", msg.data);
            } else {
              console.log("Response is empty.");
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
