var websocket = null;
var localhost = "";
var b = document.getElementById('btnWS');
var buttonClicked = false;
var webcamState = false;

// Initialize the websocket
function init() {
	if (window.location.hostname != "") {
		localhost = window.location.hostname;
	}

	doConnect();
}

function doConnect() { // makes a connection and defines callbacks
	if (webcamState == false) {
		webcamState = true;
		writeToScreen("Connecting to ws://" + localhost + ":81/ ...");
		websocket = new WebSocket("ws://" + localhost + ":81/");
		websocket.onopen = function (evt) {
			onOpen(evt)
		};
		websocket.onclose = function (evt) {
			onClose(evt)
		};
		websocket.onmessage = function (evt) {
			onMessage(evt)
		};
		websocket.onerror = function (evt) {
			onError(evt)
		};
	} else {
		writeToScreen("Disconnecting ...");
		websocket.close();
		webcamState = false;
	}
}

function onOpen(evt) { // when handshake is complete:
	writeToScreen("Connected.");
}

function onClose(evt) { // when socket is closed:
	webcamState = false;
	writeToScreen("Disconnected. Error: " + evt);
}

function onMessage(msg) {
	console.log(msg);
}

function onError(evt) { // when an error occurs
	websocket.close();
	webcamState = false;
	writeToScreen("Websocket error");

}

// Set up event listeners



// add event listeners for all the buttons in a div
function addEventListeners(strip) {
	var div = document.getElementById(strip + 'Div');
	var buttons = div.getElementsByTagName('button');
	for (var i = 0; i < buttons.length; i++) {
		buttons[i].addEventListener('click', function () {
			if (webcamState == false) {
				doConnect();
			}
			websocket.send(this.title + " " + strip);
			//make the button have background-color: #2EE59D;
			// make the other buttons have background-color: #f2f2f2;
			for (var j = 0; j < buttons.length; j++) {
				buttons[j].style.backgroundColor = "#f2f2f2";
			}
			this.style.backgroundColor = "#2EE59D";
		});
	}
	// event listener for the color picker
	div.getElementsByClassName('colorPicker')[0].addEventListener('input', function () {
		if (webcamState == false) {
			doConnect();
		}
		var color = this.value;
		// send as color r g b
		// convert hex to rgb
		var r = parseInt(color.slice(1, 3), 16);
		var g = parseInt(color.slice(3, 5), 16);
		var b = parseInt(color.slice(5, 7), 16);
		const message = "color " + strip + " " + r + " " + g + " " + b;
		websocket.send(message);
		// console.log(message);
	});
	// event listener for the brightness slider
	div.getElementsByClassName('brightnessSlider')[0].addEventListener('input', function () {
		if (webcamState == false) {
			doConnect();
		}
		var brightness = this.value;
		const message = "brightness " + strip + " " + brightness;
		websocket.send(message);
		// console.log(message);
	});


}

// add event listeners for all the buttons
addEventListeners('ceiling');
addEventListeners('desk');


// Function to display to the message box
function writeToScreen(message) {
	console.log(message);
}

// Open Websocket as soon as page loads
window.addEventListener("load", init, false);