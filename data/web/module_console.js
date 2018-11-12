var consoleModule = (function () {
  // private
  var iMaxRows = 250;
  var rows = [];
  var timerContent = null;
  var isPaused = false;
  var isEnabled = false;

  function setEnabled(enabled)
  {
    isEnabled = enabled;
  }

  function console(message) {
    while (rows.length > iMaxRows)
      rows.shift();
    if (isPaused == false)
      rows.push(JSON.stringify(message) + "<br>");
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

  function regionId()
  {
    return "region_console";
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
    console : console,
    regionId : regionId,
    setEnabled : setEnabled
  };
}());
