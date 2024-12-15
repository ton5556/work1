#include <Arduino.h>
#include <LedControl.h>
#include "pitch.h"

const int LED_DIN = 12;
const int LED_CLK = 11;
const int LED_CS = 10;

const int TRIG = 9;
const int ECHO = 8;

LedControl lc = LedControl(LED_DIN, LED_CLK, LED_CS, 1);

int duration = 0;
int distance = 0;
const int STEP = 30;
const int THRESHOLD = 30;

byte levels[8][8] = {
  {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000},
  {B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000},
  {B00000000, B00000000, B00011000, B00111100, B00111100, B00011000, B00000000, B00000000},
  {B00000000, B00011000, B00111100, B01111110, B01111110, B00111100, B00011000, B00000000},
  {B00011000, B00111100, B01111110, B11111111, B11111111, B01111110, B00111100, B00011000},
  {B00111100, B01111110, B11111111, B11111111, B11111111, B11111111, B01111110, B00111100},
  {B01111110, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B01111110},
  {B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111}
};

void setup() {
  Serial.begin(9600);

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // power on animation
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setRow(0, j, levels[i][j]);
    }
    delay(100);
  }
}

void smoothDisplay(int currentLevel, int targetLevel) {
  if (currentLevel < targetLevel) {
    // Smoothly increase the level
    for (int level = currentLevel + 1; level <= targetLevel; level++) {
      for (int i = 0; i < 8; i++) {
        lc.setRow(0, i, levels[level][i]);
      }
      delay(100); // Adjust this for smoother/slower transitions
    }
  } else if (currentLevel > targetLevel) {
    // Smoothly decrease the level
    for (int level = currentLevel - 1; level >= targetLevel; level--) {
      for (int i = 0; i < 8; i++) {
        lc.setRow(0, i, levels[level][i]);
      }
      delay(100); // Adjust this for smoother/slower transitions
    }
  }
}

void loop() {
  static int currentLevel = 0;

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);  // Round trip time
  if (duration > 0) {
    distance = duration * 340.29 / 2 / 1000;
    int targetLevel = min(max((distance - THRESHOLD) / STEP, 0), 7); // Clamp to 0-7
    smoothDisplay(currentLevel, targetLevel);
    currentLevel = targetLevel;
  }

  delay(50);
}
