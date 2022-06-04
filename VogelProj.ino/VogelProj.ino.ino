int piezo;
int LEDStoppuhr;
int LEDTimer;
int LEDWecker;
int wechselBtn;
int setBtn;
int beendenBtn;

void setup() {
  // put your setup code here, to run once:
  piezo = 6;
  LEDWecker = 7;
  LEDTimer = 9;
  LEDStoppuhr = 10;
  wechselBtn = 1;
  setBtn = 2;
  beendenBtn = 3;
  
  
  pinMode(piezo, OUTPUT);
  pinMode(LEDWecker, OUTPUT);
  pinMode(LEDtimer, OUTPUT);
  pinMode(LEDStoppuhr, OUTPUT);
}



void loop() {
  // put your main code here, to run repeatedly:

}
