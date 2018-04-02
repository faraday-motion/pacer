var consoleModule = (function () {
  // private
  var iMaxRows = 250;
  var rows = [];
  var timerContent = null;
  var isPaused = false;

  function console(message) {
    // Handle char and char array responses.
    while (rows.length > iMaxRows)
      rows.shift();
    if (isPaused == false)
      rows.push(JSON.stringify(message, undefined, 2) + "<br>");
    if (timerContent == null)
      timerContent = setInterval(setContent, 200);
  }

  function togglePause() {
    isPaused = !isPaused;
  }

  function setContent() {
    if (rows.length > 0)
    {
      var content = "";
      for (i = 0; i < rows.length; i++)
        content = rows[i] + content;
      document.getElementById("consolemessage").innerHTML = content;
    }
  }

  function clear()
  {
    while (rows.length > 0)
      rows.shift();
    $("#consolemessage").html("---------------------------------- empty ----------------------------------");
  }

  (function initialize() {
    $("#btnconsolesend").click(function(){
      var command = $("#selconsolecommand").val();
      var value = $("#txtconsolevalue").val();
      commandsModule.addCommand(command, value);
    });
    $("#btnconsolepause").click(function(){
        togglePause();
    });
    $("#btnconsoleclear").click(function(){
        clear();
    });

  })();

  return {
    // public
    console : console
  };
}());
