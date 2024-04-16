var websocket = null;
var localhost = "";
var b = document.getElementById('btnWS');
var buttonClicked = false;
var webcamState = false;

//global state variable
var state;

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
	//turn the message into a json object
	const message = JSON.parse(msg.data);
	console.log(message);
	//update the state
	state = message;
	//update the buttons
	updateButtons();
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
			//update the state and send it back
			state[strip].mode = this.title;
			websocket.send(JSON.stringify(state));
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

		//update the state and send it back
		state[strip].color.r = r;
		state[strip].color.g = g;
		state[strip].color.b = b;
		websocket.send(JSON.stringify(state));

	});
	// event listener for the brightness slider
	div.getElementsByClassName('brightnessSlider')[0].addEventListener('input', function () {
		if (webcamState == false) {
			doConnect();
		}
		var brightness = this.value;

		//update the state and send it back
		state[strip].brightness = brightness;
		
		websocket.send(JSON.stringify(state));
		// console.log(message);
	});


}


// Function to update the buttons based on the state
function updateButtons() {
	// get the buttons
	const ceilingDiv = document.getElementById('ceilingDiv');
	const deskDiv = document.getElementById('deskDiv');
	const ceilingButtons = ceilingDiv.getElementsByTagName('button');
	const deskButtons = deskDiv.getElementsByTagName('button');
	// get the color pickers
	const ceilingColorPicker = ceilingDiv.getElementsByClassName('colorPicker')[0];
	const deskColorPicker = deskDiv.getElementsByClassName('colorPicker')[0];
	// get the brightness sliders
	const ceilingBrightnessSlider = ceilingDiv.getElementsByClassName('brightnessSlider')[0];
	const deskBrightnessSlider = deskDiv.getElementsByClassName('brightnessSlider')[0];
	// update the buttons
	for (var i = 0; i < ceilingButtons.length; i++) {
		if (state.ceiling.mode == ceilingButtons[i].title) {
			ceilingButtons[i].style.backgroundColor = "#2EE59D";
		} else {
			ceilingButtons[i].style.backgroundColor = "#f2f2f2";
		}
	}
	for (var i = 0; i < deskButtons.length; i++) {
		if (state.desk.mode == deskButtons[i].title) {
			deskButtons[i].style.backgroundColor = "#2EE59D";
		} else {
			deskButtons[i].style.backgroundColor = "#f2f2f2";
		}
	}
	// update the color pickers
	ceilingColorPicker.value = rgbToHex(state.ceiling.color.r, state.ceiling.color.g, state.ceiling.color.b);
	deskColorPicker.value = rgbToHex(state.desk.color.r, state.desk.color.g, state.desk.color.b);
	// update the brightness sliders
	ceilingBrightnessSlider.value = state.ceiling.brightness;
	deskBrightnessSlider.value = state.desk.brightness;
}

// Function to convert rgb to hex
function rgbToHex(r, g, b) {
	r = r.toString(16);
	g = g.toString(16);
	b = b.toString(16);

	if (r.length == 1) {
		r = "0" + r;
	}
	if (g.length == 1) {
		g = "0" + g;
	}
	if (b.length == 1) {
		b = "0" + b;
	}

	return "#" + r + g + b;
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