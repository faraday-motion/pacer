var host = "ws://10.10.100.254:81"; 
var socket;
var receivedConfig;
var lastCommand;

function connect(){
  try{
    socket = new WebSocket(host);
        console.log('Socket Status: '+socket.readyState);
        socket.onopen = function(){
           console.log('Socket Status: '+socket.readyState+' (open)');
        }

        socket.onmessage = function(msg){
          if (msg.data.length) {
            handleResponse(msg.data);
            console.log("Response::", msg.data);
          } else {
            console.log("Response is empty.");
          }
        }
        socket.onclose = function(){
           console.log('Socket Status: '+socket.readyState+' (Closed)');
        }     
    } catch(exception){
       console.log('Error: '+ exception);
    }
}

// Automaticaly connect to the wesocket.
connect();
