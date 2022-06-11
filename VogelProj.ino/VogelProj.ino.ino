#include <LiquidCrystal.h>

// LED Display
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Alle LEDS
int ledStopwatch;
int ledTimer;
int ledAlarmClock;

// Lautsprecher und Potentiometer
int piezo;
int potentio;

// Buttons
int menuBtn;
int setBtn;
int stopBtn;

// Selektiertes Menü {0 = stoppuhr, 1 = wecker, 2 = timer}
int menu;

void setup() {
  ledAlarmClock = 7;
  ledTimer = 9;
  ledStopwatch = 10;

  piezo = 6;
  potentio = 0;

  pinMode(ledAlarmClock, OUTPUT);
  pinMode(ledTimer, OUTPUT);
  pinMode(ledStopwatch, OUTPUT);

  pinMode(piezo, OUTPUT);

  menuBtn = 1;
  setBtn = 2;
  stopBtn = 3;

  menu = -1;

  lcd.begin(16, 2);
}

void loop() {
  if (menuBtnPressed()) {
    increaseMenuSelection();
  }

  switch (menu) {
    case 0:
      lcd.clear();
      Stopwatch();
      break;
    case 1:
      lcd.clear();
      alarmClock();
      break;
    case 2:
      lcd.clear();
      timer();
      break;
  }
}

void increaseMenuSelection() {
  menu = menu + 1;
  if (menu == 3) {
    menu = 0;
  }
}

boolean menuBtnPressed() {
  return analogRead(menuBtn) != 0;
}

boolean setBtnPressed() {
  return analogRead(setBtn) != 0;
}

boolean stopBtnPressed() {
  return analogRead(stopBtn) != 0;
}

void switchLed() {
  switch (menu) {
    case 0:
      digitalWrite(ledStopwatch, HIGH);
      digitalWrite(ledAlarmClock, LOW);
      digitalWrite(ledTimer, LOW);
      break;
    case 1:
      digitalWrite(ledStopwatch, LOW);
      digitalWrite(ledAlarmClock, HIGH);
      digitalWrite(ledTimer, LOW);
      break;
    case 2:
      digitalWrite(ledStopwatch, LOW);
      digitalWrite(ledAlarmClock, LOW);
      digitalWrite(ledTimer, HIGH);
      break;
    default:
      digitalWrite(ledTimer, LOW);
      digitalWrite(ledAlarmClock, LOW);
      digitalWrite(ledStopwatch, LOW);
  }
}

void Stopwatch() {
  switchLed();
  printStoppuhrInterface();
  if (checkIfBackToMenu()) {
    return;
  }
  boolean canceld = false;
  while (!stopBtnPressed()) {
    if (!canceld) {
      runStopwatch();
      canceld = true;
      lcd.setCursor(0, 0);
      lcd.print("Entstand:");
      delay(500);
    }
    delay(100);
  }
}

void printStoppuhrInterface() {
  lcd.setCursor(0, 1);
  lcd.print("00:00");
}

void runStopwatch() {
  int intermediateTimesCount = 0;
  int seconds = 0;
  int minutes = 0;
  while (!stopBtnPressed()) {
    for (int i = 0; i < 10; i++) { // 1s Loop
      if (stopBtnPressed()) {
        return;
      } else if (shouldSaveIntermediateTime(seconds, minutes, intermediateTimesCount)) {
        printIntermediateTime(getTime(minutes, seconds), intermediateTimesCount);
        intermediateTimesCount = intermediateTimesCount + 1;
      }
      delay(100);
    }
    lcd.setCursor(0, 1);
    seconds++;
    if (isNewMinute(seconds)) {
      minutes++;
      seconds = 0;
    }

    lcd.print(getTime(minutes, seconds));
  }
}

boolean isNewMinute(int seconds) {
  return seconds % 60 == 0;
}

boolean shouldSaveIntermediateTime(int seconds, int minutes, int intermediateTimesCount) {
  return setBtnPressed() && (seconds > 0 || minutes > 0) && intermediateTimesCount < 2;
}

void printIntermediateTime(String time, int intermediateTimesCount) {
  if (intermediateTimesCount == 0) {
    lcd.setCursor(10, 0);
  } else if (intermediateTimesCount == 1) {
    lcd.setCursor(10, 1);
  }
  lcd.print(time);
}

void alarmClock() {
  switchLed();
  printAlarmClockInterface();
  int hours;
  int minutes;
  delay(300); // Hier wird gewartet, sonst wird das Menü direkt wieder gewechselt (Menü Button pressed)
  while (!setBtnPressed()) {
    lcd.setCursor(0, 1);
    hours = getHours(getPotentioValue() / 60);
    minutes = round(round(getPotentioValue()) % 60);
    lcd.print(getTime(hours, minutes));
    if (menuBtnPressed()) {
      return;
    }
    delay(50);
  }
  if (isValidTime(hours, minutes)) {
    clock(hours, minutes, 1000);
    alarm();
  }
}

void printAlarmClockInterface() {
  lcd.setCursor(0, 0);
  lcd.print("Wecker");
  lcd.setCursor(0, 1);
  lcd.print("00:00");
}

int getHours(double rawHoursValue) {
  for (int i = 0; i < 24; i++) {
    if (rawHoursValue < i + 1) {
      return i;
    }
  }
}

void clock(int firstDisplayNumber, int lastDisplayNumber, int delayCount) {
  do {
    delay(delayCount);
    lastDisplayNumber = lastDisplayNumber - 1;
    if (lastDisplayNumber == 0) {
      if (firstDisplayNumber != 0) {
        firstDisplayNumber = firstDisplayNumber - 1;
        lastDisplayNumber = 59;
      }
    }
    lcd.setCursor(0, 1);
    if (lastDisplayNumber == 0) {
      lcd.print("00:00");
    } else {
      lcd.print(getTime(firstDisplayNumber, lastDisplayNumber));
    }
  } while (lastDisplayNumber != 0);
}

void alarm() {
  while (!stopBtnPressed()) {
    lcd.setCursor(0, 1);
    lcd.print("00:00");
    tone(piezo, 262, 250);
    for (int i = 0; i < 5; i++) {
      if (stopBtnPressed()) {
        return;
      }
      if (i == 2) {
        lcd.setCursor(0, 1);
        printEmptyTime();
      }
      delay(100);
    }
  }
  return;
}

void printEmptyTime() {
  lcd.print(" ");
  lcd.print(" ");
  lcd.print(":");
  lcd.print(" ");
  lcd.print(" ");
}

void timer() {
  switchLed();
  printTimerInterface();
  int minutes;
  int seconds;
  delay(300); // Hier wird gewartet, sonst wird das Menü direkt wieder gewechselt (Menü Button pressed)
  while (!setBtnPressed()) {
    lcd.setCursor(0, 1);
    minutes = getMinutes(getPotentioValue() / 60);
    seconds = round(round(getPotentioValue()) % 60);
    lcd.print(getTime(minutes, seconds));
    if (menuBtnPressed()) {
      return;
    }
    delay(50);
  }
  if (isValidTime(minutes, seconds)) {
    clock(minutes, seconds, 1000);
    alarm();
  }
}

int getMinutes(int rawMinutesValue) {
  for (int i = 0; i < 60; i++) {
    if (rawMinutesValue < i + 1) {
      return i;
    }
  }
}

void printTimerInterface() {
  lcd.setCursor(0, 0);
  lcd.print("Timer");
  lcd.setCursor(0, 1);
  lcd.print("00:00");
}

double getPotentioValue() {
  // 24 Stunden * 60 Minuten - 1 sind 1439 Minuten, und um das auf die Auslesung des Potentiometers anzupassen wird 1439 durch 1023 geteilt
  return (analogRead(potentio) * 1.40664711);
}
String getTime(int firstDisplayNumber, int lastDisplayNumber) {
  return getTimeString(firstDisplayNumber) + ":" + getTimeString(lastDisplayNumber);
}

boolean isValidTime(int num1, int num2) {
  return num1 == 0 && num2 != 0 || num1 != 0 && num2 == 0 || num1 != 0 && num2 != 0;
}

String getTimeString(int number) {
  if (number < 10) {
    if (number == 0) {
      return String(0) + String(0); // 00
    } else {
      return String(0) + number; // 05
    }
  } else {
    return String(number); // 10
  }
}

boolean checkIfBackToMenu() {
  delay(300); // Hier wird gewartet, sonst wird das Menü direkt wieder gewechselt (Menü Button pressed)
  while (!setBtnPressed()) {
    if (menuBtnPressed()) {
      return true;
    }
    delay(20);
  }
  return false;
}
