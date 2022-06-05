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

boolean stand;

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

  stand = false;
  
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() / 1000 <= 0){
    stoppuhr();
  }
  while(analogRead(menuBtn) > 0){
    if(stand){
      timer();
      stand = false;
    }
    else{
      stoppuhr();
      stand = true;
    }
  }
}

void stoppuhr(){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("00:00");
  digitalWrite(ledTimer, LOW);
  digitalWrite(ledStoppuhr, HIGH);
  while(analogRead(setBtn) == 0){
    if(analogRead(menuBtn) != 0){
      return;
    }
  }
  lcd.clear();
  while(analogRead(stoppBtn) <= 0){
    String anzeige = "";
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    timeS++;
    if(timeS % 60 == 0){
      timeM++;
    }
    String sek;
    String minu;
    sek = timeS;
    if(timeS < 10){
      sek = "0" + timeS;
      if(timeS == 0){
        sek = "00"; 
      }
    }
    minu = timeM;
    if(timeM < 10){
      minu = "0" + timeM;
      if(timeM == 0){
        minu = "00";
      } 
    }
    anzeige = minu + ":" + sek; 
    lcd.print(anzeige);
  }
  timeM = 0;
  timeS = 0;
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("00:00");
}

void timer(){
  digitalWrite(ledStoppuhr, LOW);
  digitalWrite(ledTimer, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Zeit setzen");
}
