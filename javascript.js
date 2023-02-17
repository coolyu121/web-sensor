document.getElementById('ID_LED_0').addEventListener('click', led_changed);
  document.getElementById('ID_LED_1').addEventListener('click', led_changed);
  document.getElementById('ID_LED_2').addEventListener('click', led_changed);

  var slider = document.getElementById('ID_LED_INTENSITY');
  var output = document.getElementById('ID_LED_INTENSITY_VALUE');
  slider.addEventListener('change', slider_changed);

  var Socket;
  
  function init() {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    Socket.onmessage = function(event) {
      processCommand(event);
    };
  }
  
  function led_changed() {
	var l_LED_selected = 0;
	if(document.getElementById('ID_LED_1').checked == true) {
	  l_LED_selected = 1;
	} else if(document.getElementById('ID_LED_2').checked == true) {
	  l_LED_selected = 2;
	}
	console.log(l_LED_selected);
    var msg = { type: 'LED_selected', value: l_LED_selected};
	Socket.send(JSON.stringify(msg)); 
  }
  
  function slider_changed () {
    var l_LED_intensity = slider.value;
	console.log(l_LED_intensity);
	var msg = { type: 'LED_intensity', value: l_LED_intensity};
	Socket.send(JSON.stringify(msg)); 
  }

  function processCommand(event) {
	var obj = JSON.parse(event.data);
    var type = obj.type;
	if (type.localeCompare("LED_intensity") == 0) { 
	  var l_LED_intensity = parseInt(obj.value); 
	  console.log(l_LED_intensity); 
	  slider.value = l_LED_intensity; 
	  output.innerHTML = l_LED_intensity;
	}
  else if(type.localeCompare("graph_update")==0){
    console.log(obj.value);
    myChart.data.datasets[0].data = obj.value;
    myChart.update();
  }
	else if(type.localeCompare("LED_selected") == 0) { 
	  var l_LED_selected = parseInt(obj.value); 
	  console.log(l_LED_selected); 
	  if(l_LED_selected == 0) { 
	    document.getElementById('ID_LED_0').checked = true; 
      } else if (l_LED_selected == 1) { 
	    document.getElementById('ID_LED_1').checked = true; 
	  } else if (l_LED_selected == 2) { 
	    document.getElementById('ID_LED_2').checked = true; 
	  }
	} 
  }
  window.onload = function(event) {
    init();
  }
  
var array_length = 10;
var xValues=[];
for (let i = 0; i < array_length; i++) {
  xValues[i] = i;
}

var yValues = [];

const myChart = new Chart("myChart", {
  type: "line",
  data: {
    labels: xValues,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: yValues
    }]
  },
  options: {
    legend: {display: false},
    scales: {
      yAxes: [{ticks: {min: 0, max:100}}],
    }
  }
});