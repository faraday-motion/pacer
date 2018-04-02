var websocketModule = (function (parameters) {
  // private
  parameters = parameters || {};
  var host = (parameters.host === undefined ? "ws://10.10.100.254:81" : parameters.host);
  var connectTimeout = (parameters.connectTimeout === undefined ? 3000 : parameters.connectTimeout);;

  var isConnected = false;
  var socket;

  function connect(connectCallback, messageRecieved){
    function triggerCallback()
    {
      if (connectCallback)
        connectCallback(isConnected);
    }

    try{
      if (socket)
        socket.close();
      socket = new WebSocket(host);
          console.log('WebSocket Socket Status: ' + socket.readyState);

          socket.onopen = function(){
            isConnected = true;
            console.log('onopen Socket Status: ' + socket.readyState + ' (open)');
            triggerCallback();
          }

          socket.onmessage = function(msg){
            isConnected = true;
            if (msg.data.length) {
              var obj = JSON.parse(msg.data);
              if (obj.msg == "ping")
                sendRaw("{\"ping\":\"pong\"}");              }
              else
                messageRecieved(obj);
              console.log("Response::", msg.data);
            } else {
              console.log("Response is empty.");
            }
          }

          socket.onerror = function(){
            isConnected = false;
            console.log('onerror Socket Status: ' + socket.readyState + ' (Closed)');
            triggerCallback();
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
        triggerCallback();
        console.log('exception: ' + exception);
      }
  }

  function send(command, value)
  {
    sendRaw("{\"command\":" + JSON.parse(command) + ",\"value\":" + JSON.parse(value) + "}");
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
    send : send
  };
}());
