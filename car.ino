#include "pinout.h"
#include "debounce.h"

int           ledState       = LOW;
unsigned long calibrateDelay = 2000;

const byte snr_light1_smoothnumber = 50;
int snr_light1_readings[snr_light1_smoothnumber];
int snr_light1readIndex = 0;
int snr_light1total = 0;
int snr_light1average = 0;

float out;

void getAvg() {
  int min = 1024;
  int max = 0;

  int m = 15;
  int i = 0;
  int tot = 0;
  int readings[m];
  while (i < m || true) {
    debouceTick();
    if (btnState == HIGH && btnFlank) {
      min = 1024;
      max = 0;
        }

    if (i >= m) {
      i = 0;
      tot = 0;
    }

      if (!out) out = analogRead(snr_light1);
      out += (analogRead(snr_light1) - out) * 0.3;

      readings[i] = out;

      tot += out;

      int t = tot / (i + 1);
      if (min > t) min = t;
      if (max < t) max = t;

      Serial.print(out);
      Serial.print(", ");
      Serial.print(min);
      Serial.print(", "),
      Serial.print(max);
      Serial.print(", "),
      Serial.print((max + min) / 2);
      digitalWrite(led, (max + min) / 2 < t);
      Serial.print(", ");
      Serial.println(t);

        i++;
  }

  while (true) {
    Serial.print(out);
    Serial.print(", ");
    Serial.println(tot / i);
  }
}

void smoothRun() {
  if (!out) out = analogRead(snr_light1);
  out += (analogRead(snr_light1) - out) * 0.3;

  snr_light1total = snr_light1total - snr_light1_readings[snr_light1readIndex];
  snr_light1_readings[snr_light1readIndex] = out;
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

  getAvg();

  smoothRun();
  Serial.print(out);
  Serial.print(", ");
  Serial.println(snr_light1average);
}
