#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define CYCLE_DELAY 10

#define MOTOR_ESQ_1 16
#define MOTOR_ESQ_2 0
#define MOTOR_ESQ_3 4
#define MOTOR_ESQ_4 15

#define MOTOR_DIR_1 19
#define MOTOR_DIR_2 5
#define MOTOR_DIR_3 18
#define MOTOR_DIR_4 17

#define STEPS_PER_REVOLUTION  2048

#define LED_PIN               2
#define SHORT_BLINK_INTERVAL  200
#define LONG_BLINK_INTERVAL   1000

#define INITIAL_RPM           10

#define IR_SENSOR_PIN         13

extern String active_cmd;

extern const int rpm;

extern unsigned long lastBlink;
extern unsigned long blinkInterval;
extern bool ledState;

extern String last_motion_cmd;

extern const char* ssid;
extern const char* password;
extern const short port;

extern const char index_html[] PROGMEM;

#endif
