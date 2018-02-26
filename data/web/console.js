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

var iMaxRows = 250;
var rows = [];
var timerContent = null;
var isPaused = false;

function printResponse(response) {
  // Handle char and char array responses.
  if (requestCommand == 8008 || requestCommand == 8011) {
    response = parseCtrlId(response);
  }
  while (rows.length > iMaxRows)
    rows.shift();
  if (isPaused == false)
    rows.push(JSON.parse(JSON.stringify(response, undefined, 2)) + "<br>");
  if (timerContent == null)
    timerContent = setTimeout(setContent, 250);
}

function pause() {
  isPaused = !isPaused;
}

function setContent() {
  var content = "";
  for (i = 0; i < rows.length; i++) {
    content = rows[i] + content;
  }
  document.getElementById("response").innerHTML = content;
  timerContent = setTimeout(setContent, 100);
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
