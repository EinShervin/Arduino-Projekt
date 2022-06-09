#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ledStopwatch;
int ledTimer;
int ledAlarmClock;

int piezo;
int potentio;

int menuBtn;
int setBtn;
int stoppBtn;

int timeS;
int timeM;

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
  stoppBtn = 3;

  timeS = 0;
  timeM = 0;

  menu = -1;

  lcd.begin(16, 2);
}

void loop() {
  if (analogRead(menuBtn) > 0) {
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
  if (checkIfMenuIsPressed()) {
    return;
  }
  boolean canceld = false;
  while (analogRead(stoppBtn) <= 0) {
    if (!canceld) {
      time();
      canceld = true;
      lcd.setCursor(0, 0);
      lcd.print("Entstand:");
      delay(500);
    }
    delay(100);
  }
  timeM = 0;
  timeS = 0;
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("00:00");
}

void time() {
  int zTimes = 0;
  while (analogRead(stoppBtn) <= 0) {
    for (int i = 0; i < 10; i++) {
      if (analogRead(stoppBtn) > 0) {
        return;
      } else if (setBtnPressed() && (timeS > 0 || timeM > 0) && zTimes < 2) {
        if (zTimes == 0) {
          lcd.setCursor(10, 0);
        } else if (zTimes == 1) {
          lcd.setCursor(10, 1);
        }
        lcd.print(getTime());
        zTimes = zTimes + 1;
      }
      delay(100);
    }
    lcd.setCursor(0, 1);
    timeS++;
    if (timeS % 60 == 0) {
      timeM = timeM + 1;
      timeS = 0;
    }

    lcd.print(getTime());
  }
}

String getTime() {
  return getMin() + ":" + getSek();
}

String getSek() {
  if (timeS < 10) {
    if (timeS == 0) {
      return String(0) + String(0); // 0 Sekunden
    } else {
      return "0" + String(timeS); // 05 Sekudnen z.B.
    }
  } else {
    return String(timeS);
  }
}

String getMin() {
  if (timeM < 10) {
    if (timeM == 0) {
      return String(0) + String(0);
    } else {
      return String(0) + timeM;
    }
  } else {
    return String(timeM);
  }
}

void wecker() {
  switchLed();
  lcd.setCursor(0, 0);
  lcd.print("Wecker");
  lcd.setCursor(0, 1);
  lcd.print("00:00");
  delay(500);
  while (!setBtnPressed()) {
    lcd.setCursor(0, 1);
    lcd.print(getWeckerTime());
    if (menuBtnPressed()) {
      timeS = 0;
      timeM = 0;
      return;
    }
    delay(50);
  }
  if (timeM == 0 && timeS != 0 || timeM != 0 && timeS == 0 || timeM != 0 && timeS != 0) {
    do {
      delay(1000);
      timeS = timeS - 1;
      if (timeS == 0) {
        if (timeM != 0) {
          timeM = timeM - 1;
          timeS = 59;
        }
      }
      lcd.setCursor(0, 1);
      if (timeS == 0) {
        lcd.print("00:00");
      } else {
        lcd.print(getTime());
      }
    } while (timeS != 0);
    end();
  }

  timeS = 0;
  timeM = 0;
}

void end() {
  while (analogRead(stoppBtn) <= 0) {
    lcd.setCursor(0, 1);
    lcd.print("00:00");
    tone(piezo, 262, 250);
    for (int i = 0; i < 5; i++) {
      if (analogRead(stoppBtn) > 0) {
        return;
      }
      if (i == 2) {
        lcd.setCursor(0, 1);
        lcd.print("  :  ");
      }
      delay(100);
    }
  }
  return;
}

String getWeckerTime() {
  double result = analogRead(potentio);
  double result2 = (result * 1.40664711);
  timeM = getHour(result2 / 60);
  timeS = round(round(result2) % 60);
  return getTime();
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
  delay(500);
  while (!setBtnPressed()) {
    lcd.setCursor(0, 1);
    lcd.print(getWeckerTime());
    if (menuBtnPressed()) {
      timeS = 0;
      timeM = 0;
      return;
    }
    delay(50);
  }
  if (timeM == 0 && timeS != 0 || timeM != 0 && timeS == 0 || timeM != 0 && timeS != 0) {
    do {
      delay(1000);
      timeS = timeS - 1;
      if (timeS == 0) {
        if (timeM != 0) {
          timeM = timeM - 1;
          timeS = 59;
        }
      }
      lcd.setCursor(0, 1);
      if (timeS == 0) {
        lcd.print("00:00");
      } else {
        lcd.print(getTime());
      }
    } while (timeS != 0);
    end();
  }

  timeS = 0;
  timeM = 0;
}

boolean checkIfMenuIsPressed() {
  delay(500);
  while (!setBtnPressed()) {
    if (menuBtnPressed()) {
      return true;
    }
    delay(20);
  }
  return false;
}
