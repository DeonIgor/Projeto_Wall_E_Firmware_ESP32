#include "config.h"

String active_cmd = "stop";

const int rpm = INITIAL_RPM;

unsigned long lastBlink = 0;
unsigned long blinkInterval = LONG_BLINK_INTERVAL;
bool ledState = false;

String last_motion_cmd = "stop";


const char* ssid = "Wall-E-Robot";
const char* password = "12345678";
const short port = 80;

const char index_html[] PROGMEM = R"rawliteral(
<html>
	<head>
		<title>Controle Wall-E</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<style>
			body {
			  font-family: Arial, sans-serif;
			  text-align: center;
			  background-color: #1c1c1c;
			  color: #fff;
			  margin: 0;
			  padding: 20px;
			}

			h1 {
			  margin-bottom: 40px;
			  color: #00bfff;
			}
			
			.speed_controller {
			  margin-top: 60px;
			}

			.speed_controller label {
			  font-size: 18px;
			  display: block;
			  margin-bottom: 10px;
			}

			.speed_controller #speedDisplay {
			  color: #00bfff;
			}

			.speed_controller input[type="range"] {
			  width: 60%;
			  height: 10px;
			  border-radius: 5px;
			  outline: none;
			  accent-color: #00bfff;
			}

			.movement_controller {
			  display: grid;
			  grid-template-columns: 100px 100px 100px;
			  grid-template-rows: 100px 100px 100px;
			  gap: 0;
			  justify-content: center;
			  align-items: center;
			}
			
			.btn {
			  width: 100px;
			  height: 100px;
			  cursor: pointer;
			  opacity: 0.9;
			  transition: transform 0.2s, filter 0.2s;
			}

			.btn svg {
			  width: 100%;
			  height: 100%;
			}

			.btn:active {
			  transform: scale(1.1);
			  filter: brightness(1.2);
			}

			.up    { grid-area: 1 / 2; }
			.left  { grid-area: 2 / 1; }
			.right { grid-area: 2 / 3; }
			.down  { grid-area: 3 / 2; }

			.up svg    { transform: rotate(0deg); }
			.right svg { transform: rotate(90deg); }
			.down svg  { transform: rotate(180deg); }
			.left svg  { transform: rotate(-90deg); }

			.stop {
			  grid-area: 2 / 2;
			  width: 100px;
			  height: 100px;
			  border-radius: 50%;
			  background-color: yellow;
			  transition: transform 0.2s, filter 0.2s;
			}

			.stop:active {
			  transform: scale(1.1);
			  filter: brightness(1.2);
			}
		</style>
	</head>
	
	<body>
		<h1>Wall-E</h1>
		
		<div class="movement_controller">
			<!-- Forward -->
			<div class="btn up" ontouchstart="send('forward')" ontouchend="send('stop')" onmousedown="send('forward')" onmouseup="send('stop')">
				<svg viewBox="0 0 100 100">
					<path d="M50 10 L90 90 Q50 70 10 90 Z" fill="#00bfff"/>
				</svg>
			</div>

			<!-- Left -->
			<div class="btn left" ontouchstart="send('left')" ontouchend="send('stop')" onmousedown="send('left')" onmouseup="send('stop')">
				<svg viewBox="0 0 100 100">
					<path d="M50 10 L90 90 Q50 70 10 90 Z" fill="#00bfff"/>
				</svg>
			</div>

			<!-- Stop -->
			<div class="stop" ontouchstart="send('stop')" onmousedown="send('stop')"></div>

			<!-- Right -->
			<div class="btn right" ontouchstart="send('right')" ontouchend="send('stop')" onmousedown="send('right')" onmouseup="send('stop')">
				<svg viewBox="0 0 100 100">
					<path d="M50 10 L90 90 Q50 70 10 90 Z" fill="#00bfff"/>
				</svg>
			</div>

			<!-- Backward -->
			<div class="btn down" ontouchstart="send('backward')" ontouchend="send('stop')" onmousedown="send('backward')" onmouseup="send('stop')">
				<svg viewBox="0 0 100 100">
					<path d="M50 10 L90 90 Q50 70 10 90 Z" fill="#00bfff"/>
				</svg>
			</div>
		</div>

		<div class="speed_controller">
			<label for="speed">
				Velocidade: <span id="speedDisplay" style="color: #00bfff;">10</span> RPM
			</label>
			<input type="range" id="speed" min="1" max="20" value="10" oninput="updateSpeed(this.value)">
		</div>

		
		<script>
			let ws;
			function initWS() {
				ws = new WebSocket('ws://' + location.host + '/ws');
				ws.onopen = () => console.log('WebSocket conectado');
				ws.onclose = () => {
					console.log('WebSocket desconectado. Tentando reconectar...');
					setTimeout(initWS, 1000);
				};
			}

			function send(cmd) {
				if (ws && ws.readyState === WebSocket.OPEN) {
					ws.send(cmd);
				}
			}
			
			function updateSpeed(value) {
				document.getElementById("speedDisplay").innerText = value;
				send("speed:" + value);
			}

			window.onload = initWS;
		</script>
	</body>
</html>
)rawliteral";