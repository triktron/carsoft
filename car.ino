#include "pinout.h"
#include "debounce.h"

int           ledState       = LOW;
unsigned long calibrateDelay = 2000;

const byte snr_light1_smoothnumber = 150;
int snr_light1_readings[snr_light1_smoothnumber];
int snr_light1readIndex = 0;
int snr_light1total = 0;
int snr_light1average = 0;

void smoothRun() {
  snr_light1total = snr_light1total - snr_light1_readings[snr_light1readIndex];
  snr_light1_readings[snr_light1readIndex] = analogRead(snr_light1);
  snr_light1total = snr_light1total + snr_light1_readings[snr_light1readIndex];
  snr_light1readIndex = snr_light1readIndex + 1;

  if (snr_light1readIndex >= snr_light1_smoothnumber) {
    snr_light1readIndex = 0;
  }

  snr_light1average = snr_light1total / snr_light1_smoothnumber;
}

void checkCalibrate() {
  unsigned long stime = millis();
  debouceTick(true);
  while (btnState == HIGH) debouceTick();
  if ((millis() - stime) > calibrateDelay) {
    for (int i = 0; i < 4;i++) {
      digitalWrite(led, HIGH);
      delay(200);
      digitalWrite(led, LOW);
      delay(200);
    }
  }
}

void setup() {
  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
  for (int thisReading = 0; thisReading < snr_light1_smoothnumber; thisReading++) {
    snr_light1_readings[thisReading] = 0;
  }

  checkCalibrate();

  digitalWrite(led, ledState);
}

void loop() {
  debouceTick();
  if (btnState == HIGH && btnFlank) {
        ledState = !ledState;
      }
  digitalWrite(led, ledState);

  smoothRun();
  Serial.println(analogRead(snr_light1average));
}
