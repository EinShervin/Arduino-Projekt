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

  nextMenu = false;

  menu = -1;

  Serial.begin(9600);
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
      lcd.setCursor(0, 1);
      lcd.print("stoppuhr...");
      stoppuhr();
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("wecker...");
      // wecker();
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

boolean srtBtnPressed() {
  return analogRead(setBtn) != 0;
}

void stoppuhr() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("00:00");
  while (!srtBtnPressed()) {
    if (menuBtnPressed()) {
      return;
    }
  }
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("loading...");
  Serial.print(analogRead(stoppBtn));
  while (analogRead(stoppBtn) <= 0) {
    for (int i = 0; i < 10; i++) {
      if (analogRead(stoppBtn) > 0) {
        timeM = 0;
        timeS = 0;
        nextMenu = true;
        return;
      } else if (srtBtnPressed()) {
         
      }
      delay(100);
    }
    lcd.clear();
    lcd.setCursor(0, 1);
    timeS++;
    if (timeS % 60 == 0) {
      timeM = timeM + 1;
      timeS = 0;
    }

    lcd.print(getMin() + ":" + getSek());
  }
  timeM = 0;
  timeS = 0;
  nextMenu = true;
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("00:00");
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

void timer() {
  digitalWrite(ledStoppuhr, LOW);
  digitalWrite(ledTimer, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Zeit setzen");
}
