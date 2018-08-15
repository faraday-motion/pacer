var graphModule = (function () {
  // private
  var sensors = null;
  var isEnabled = false;

  function setEnabled(enabled)
  {
    isEnabled = enabled;
  }

  function setupPowerGraph()
  {
    var smoothiePower = new SmoothieChart();
    smoothiePower.streamTo(document.getElementById("canpower"), 200);

    // Data
    var line1 = new TimeSeries();
    var line2 = new TimeSeries();
    var line3 = new TimeSeries();
    var line4 = new TimeSeries();

    setInterval(function() {
      if (sensors)
      {
        line1.append(new Date().getTime(), sensors.accel);
        line2.append(new Date().getTime(), sensors.brake);
        line3.append(new Date().getTime(), sensors.left);
        line4.append(new Date().getTime(), sensors.right);
      }
    }, 200);

    smoothiePower.addTimeSeries(line1, { strokeStyle:'rgb(0, 255, 0)', fillStyle:'rgba(0, 255, 0, 0.4)', lineWidth:3 });
    smoothiePower.addTimeSeries(line2, { strokeStyle:'rgb(255, 0, 100)', fillStyle:'rgba(255, 0, 100, 0.3)', lineWidth:3 });
    smoothiePower.addTimeSeries(line3, { strokeStyle:'rgb(255, 255, 0)', fillStyle:'rgba(255, 255, 0, 0.3)', lineWidth:3 });
    smoothiePower.addTimeSeries(line4, { strokeStyle:'rgb(0, 0, 255)', fillStyle:'rgba(0, 0, 255, 0.3)', lineWidth:3 });
  }

  function setupPerformanceGraph()
  {
    var smoothiePerformance = new SmoothieChart({millisPerPixel:1000, tooltip:true, grid:{millisPerLine:10000,verticalSections:1}});
    smoothiePerformance.streamTo(document.getElementById("canperformance"), 200);

    // Data
    var line1 = new TimeSeries();
    var line2 = new TimeSeries();
    var line3 = new TimeSeries();
    var line4 = new TimeSeries();

    setInterval(function() {
      if (sensors)
      {
        line1.append(new Date().getTime(), sensors.vesc_0_v_in);
        line2.append(new Date().getTime(), sensors.vesc_0_t_pcb);
        line3.append(new Date().getTime(), sensors.vesc_0_c_mot);
        line4.append(new Date().getTime(), sensors.vesc_0_duty);
      }
    }, 200);

    smoothiePerformance.addTimeSeries(line1, { strokeStyle:'rgb(0, 255, 0)', lineWidth:3 });
    smoothiePerformance.addTimeSeries(line2, { strokeStyle:'rgb(255, 0, 100)', lineWidth:3 });
    smoothiePerformance.addTimeSeries(line3, { strokeStyle:'rgb(255, 255, 0)', lineWidth:3 });
    smoothiePerformance.addTimeSeries(line4, { strokeStyle:'rgb(0, 0, 255)', lineWidth:3 });
  }

  function setupChargeGraph()
  {
    var smoothieCharge = new SmoothieChart({millisPerPixel:1000, tooltip:true, grid:{millisPerLine:10000,verticalSections:1}});
    smoothieCharge.streamTo(document.getElementById("cancharge"), 200);

    // Data
    var line1 = new TimeSeries();
    var line2 = new TimeSeries();
    var line3 = new TimeSeries();

    setInterval(function() {
      if (sensors)
      {
        line1.append(new Date().getTime(), sensors.vesc_0_a_charged);
        line2.append(new Date().getTime(), sensors.vesc_0_w_hours);
        line3.append(new Date().getTime(), sensors.vesc_0_rpm);
      }
    }, 200);

    smoothieCharge.addTimeSeries(line1, { strokeStyle:'rgb(0, 255, 0)', lineWidth:3 });
    smoothieCharge.addTimeSeries(line2, { strokeStyle:'rgb(255, 0, 100)', lineWidth:3 });
    smoothieCharge.addTimeSeries(line3, { strokeStyle:'rgb(255, 255, 0)', lineWidth:3 });
  }

  function regionId()
  {
    return "region_data";
  }

  function setSensors(sensorValues)
  {
    sensors = sensorValues;
  }

  (function initialize() {

    $(document).ready(function(){
      setupPowerGraph();
      setupPerformanceGraph();
      setupChargeGraph();
    });
  })();

  return {
    // public
    regionId : regionId,
    setSensors : setSensors,
    setEnabled : setEnabled
  };
}());
