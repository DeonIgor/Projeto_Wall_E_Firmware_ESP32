#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>
#include "config.h"

// Criação do servidor web na porta 80
AsyncWebServer server(port);
AsyncWebSocket ws("/ws");

// Define os motores com os pinos conectados ao ULN2003
Stepper motor_left(STEPS_PER_REVOLUTION, MOTOR_ESQ_1, MOTOR_ESQ_2, MOTOR_ESQ_3, MOTOR_ESQ_4);
Stepper motor_right(STEPS_PER_REVOLUTION, MOTOR_DIR_1, MOTOR_DIR_2, MOTOR_DIR_3, MOTOR_DIR_4);

void handleCommand(String cmd) {
  bool obstacleDetected = digitalRead(IR_SENSOR_PIN) == LOW;
  bool isMotion = (cmd == "forward" || cmd == "backward" || cmd == "left" || cmd == "right");

  if (obstacleDetected && isMotion && cmd == last_motion_cmd) return;

  active_cmd = cmd;

  if (isMotion) {
    last_motion_cmd = cmd;
  }
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len) {
      data[len] = 0;
      String cmd = (char*)data;

      if (cmd.startsWith("speed:")) {
        int newSpeed = cmd.substring(6).toInt();
        Serial.printf("Nova velocidade: %d RPM\n", newSpeed);
        motor_left.setSpeed(newSpeed);
        motor_right.setSpeed(newSpeed);
      }
      else {
        handleCommand(cmd);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  pinMode(MOTOR_ESQ_1, OUTPUT);
  pinMode(MOTOR_ESQ_2, OUTPUT);
  pinMode(MOTOR_ESQ_3, OUTPUT);
  pinMode(MOTOR_ESQ_4, OUTPUT);

  pinMode(MOTOR_DIR_1, OUTPUT);
  pinMode(MOTOR_DIR_2, OUTPUT);
  pinMode(MOTOR_DIR_3, OUTPUT);
  pinMode(MOTOR_DIR_4, OUTPUT);

  // Define IP fixo (sem isso funciona da mesma forma)
  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  // Configura os IPs do ESP
  WiFi.softAPConfig(local_ip, gateway, subnet);

  // Configura o Access Point
  WiFi.softAP(ssid, password);

  // Imprime o endereço IP com a porta em que o ESP espera conexão
  Serial.println("AP iniciado: " + WiFi.softAPIP().toString() + ":" + port);

  // Registra um handler para os eventos do web socket
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Espera a conexão na raiz do servidor para retornar a página HTML
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();

  // Configura motores
  motor_left.setSpeed(INITIAL_RPM);
  motor_right.setSpeed(INITIAL_RPM);
}

void loop() {

  if(active_cmd == "forward") {
    motor_left.step(1);
    motor_right.step(1);
    blinkInterval = SHORT_BLINK_INTERVAL;
  }
  else if(active_cmd == "backward") {
    motor_left.step(-1);
    motor_right.step(-1);
    blinkInterval = SHORT_BLINK_INTERVAL;
  }
  else if(active_cmd == "left") {
    motor_left.step(-1);
    motor_right.step(1);
    blinkInterval = SHORT_BLINK_INTERVAL;
  }
  else if(active_cmd == "right") {
    motor_left.step(1);
    motor_right.step(-1);
    blinkInterval = SHORT_BLINK_INTERVAL;
  }
  else {
    blinkInterval = LONG_BLINK_INTERVAL;
  }

  if (millis() - lastBlink >= blinkInterval) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    lastBlink = millis();
  }

  delay(CYCLE_DELAY);
}
