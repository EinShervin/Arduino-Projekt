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

// Selektiertes Menü
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
      stoppuhr();
      break;
    case 1:
      lcd.clear();
      wecker();
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

void stoppuhr() {
  switchLed();
  lcd.setCursor(0, 1);
  lcd.print("00:00");
  if (checkIfBackToMenu()) {
    return;
  }
  boolean canceld = false;
  while (!stopBtnPressed()) {
    if (!canceld) {
      time();
      canceld = true;
      lcd.setCursor(0, 0);
      lcd.print("Entstand:");
      delay(500);
    }
    delay(100);
  }
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("00:00");
}

void time() {
  int intermediateTimes = 0;
  int seconds = 0;
  int minutes = 0;
  while (!stopBtnPressed()) {
    for (int i = 0; i < 10; i++) {
      if (stopBtnPressed()) {
        return;
      } else if (setBtnPressed() && (seconds > 0 || minutes > 0) && intermediateTimes < 2) {
        if (intermediateTimes == 0) {
          lcd.setCursor(10, 0);
        } else if (intermediateTimes == 1) {
          lcd.setCursor(10, 1);
        }
        lcd.print(getTime(minutes, seconds));
        intermediateTimes = intermediateTimes + 1;
      }
      delay(100);
    }
    lcd.setCursor(0, 1);
    seconds++;
    if (seconds % 60 == 0) {
      minutes = minutes + 1;
      seconds = 0;
    }

    lcd.print(getTime(minutes, seconds));
  }
}

String getTime(int firstDisplayNumber, int lastDisplayNumber) {
  return getTimeString(firstDisplayNumber) + ":" + getTimeString(lastDisplayNumber);
}

String getTimeString(int number) {
  if (number < 10) {
    if (number == 0) {
      return String(0) + String(0); // 00 Sekunden
    } else {
      return String(0) + number; // 05 Sekunden
    }
  } else {
    return String(number);
  }
}

void wecker() {
  switchLed();
  lcd.setCursor(0, 0);
  lcd.print("Wecker");
  lcd.setCursor(0, 1);
  lcd.print("00:00");
  delay(500);
  int hours;
  int minutes;
  while (!setBtnPressed()) {
    lcd.setCursor(0, 1);
    hours = getHour(getPotentioValue() / 60);
    minutes = round(round(getPotentioValue()) % 60);
    lcd.print(getTime(hours, minutes));
    if (menuBtnPressed()) {
      return;
    }
    delay(50);
  }
  if (hours == 0 && minutes != 0 || hours != 0 && minutes == 0 || hours != 0 && minutes != 0) {
    count(hours, minutes, 1000);
    end();
  }
}

void count(int firstDisplayNumber, int lastDisplayNumber, int delayCount) {
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

void end() {
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
        lcd.print(" ");
        lcd.print(" ");
        lcd.print(":");
        lcd.print(" ");
        lcd.print(" ");
        }
      delay(100);
    }
  }
  return;
}

int getHour(double rawHourValue) {
  for (int i = 0; i < 24; i++)
    if (rawHourValue < i + 1) {
      return i;
    }
}

void timer() {
  switchLed();
  lcd.setCursor(0, 0);
  lcd.print("Zeit setzen");
  lcd.setCursor(0, 1);
  lcd.print("00:00");
  int minutes;
  int seconds;
  delay(500);
  while (!setBtnPressed()) {
    lcd.setCursor(0, 1);
    minutes = getHour(getPotentioValue() / 60);
    seconds = round(round(getPotentioValue()) % 60);
    lcd.print(getTime(minutes, seconds));
    if (menuBtnPressed()) {
      return;
    }
    delay(50);
  }
  if (minutes == 0 && seconds != 0 || minutes != 0 && seconds == 0 || minutes != 0 && seconds != 0) {
    count(minutes, seconds, 1000);
    end();
  }
}

double getPotentioValue() {
  // 24 Stunden * 60 Minuten - 1 sind 1439 Minuten, und um das auf die Auslesung des Potentiometers anzupassen wird 1439 durch 1023 geteilt
  return (analogRead(potentio) * 1.40664711);
}

boolean checkIfBackToMenu() {
  delay(500);
  while (!setBtnPressed()) {
    if (menuBtnPressed()) {
      return true;
    }
    delay(20);
  }
  return false;
}
