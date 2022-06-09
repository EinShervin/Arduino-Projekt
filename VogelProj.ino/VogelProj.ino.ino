#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int piezo;
int ledStoppuhr;
int ledTimer;
int ledWecker;

int menuBtn;
int setBtn;
int stoppBtn;

int timeS;
int timeM;

int menu;
boolean nextMenu;

void setup() {
  // put your setup code here, to run once:
  piezo = 6;
  ledWecker = 7;
  ledTimer = 9;
  ledStoppuhr = 10;

  pinMode(piezo, OUTPUT);
  pinMode(ledWecker, OUTPUT);
  pinMode(ledTimer, OUTPUT);
  pinMode(ledStoppuhr, OUTPUT);

  menuBtn = 1;
  setBtn = 2;
  stoppBtn = 3;

  timeS = 0;
  timeM = 0;

  menu = -1;
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(menuBtn) > 0 || nextMenu) {
    menu = menu + 1;
    if (menu == 3) {
      menu = 0;
    }
  }

  switch (menu) {
    case 0:
      digitalWrite(ledTimer, LOW);
      digitalWrite(ledStoppuhr, HIGH);
      lcd.clear();
      stoppuhr();
      break;
    case 1:
      lcd.clear();
      wecker();
      break;
    case 2:
      // timer();
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("timer...");
      break;
  }
}

boolean menuBtnPressed() {
  return analogRead(menuBtn) != 0;
}

boolean setBtnPressed() {
  return analogRead(setBtn) != 0;
}

void stoppuhr() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("00:00");
  while (!setBtnPressed()) {
    if (menuBtnPressed()) {
      return;
    }
    delay(20);
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
  lcd.setCursor(0, 0);
  lcd.print("Wecker");
  lcd.setCursor(0, 1);
  lcd.print(getTime());
  while (!setBtnPressed()) {
    if (menuBtnPressed()) {
      return;
    }
    delay(50);
  }
}

void timer() {
  digitalWrite(ledStoppuhr, LOW);
  digitalWrite(ledTimer, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Zeit setzen");
}
