// Console function. 

var requestCommand; // holds the previously requested command.

function handleResponse(payload)
{
  response = payload;
  printResponse(response);
}

function sendRequest()
{
  requestCommand = document.getElementById('command').value;
  var payload    = document.getElementById('payload').value;
  console.log("{\"command\":" + JSON.parse(requestCommand) + ",\"value\":" + JSON.parse(payload) + "}");
  socket.send("{\"command\":" + JSON.parse(requestCommand) + ",\"value\":" + JSON.parse(payload) + "}");
}

function printResponse(response) {

  // Handle char and char array responses.
  if (requestCommand == 8008 || requestCommand == 8011) {
    response = parseCtrlId(response);
  }
  document.getElementById("response").innerHTML = JSON.parse(JSON.stringify(response, undefined, 2)) + "<br>" + document.getElementById("response").innerHTML;
}


function parseCtrlId(data)
{
  let ctrls = []; 
  var controllers = data.split('|');
  if (controllers.length) {
    for (a = 0; a < controllers.length; a++ ) {
      var chars = controllers[a].split(":");
      let ctrlId = '';
      for (var i = 0; i < chars.length; i++) {
        ctrlId = ctrlId + String.fromCharCode(chars[i]);
      }
      if (ctrlId.length == 5) { // an id must have 5 chars.
        ctrls.push(ctrlId);
      }
    }
  }
  return ctrls;
}