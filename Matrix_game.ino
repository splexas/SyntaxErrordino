#include <LedControl.h>
int DIN = 11;
int CS = 7;
int CLK = 13;
int buttonPin = 5;   
int lcnum = 0;

LedControl lc=LedControl(DIN, CLK, CS,0);

void setup() {
  pinMode(buttonPin, INPUT);

  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.shutdown(1,false);
  lc.setIntensity(1,0);
  lc.clearDisplay(0);
  lc.clearDisplay(1);
}

bool wasbreaked = false;
int row=0;
int col=0;
int dly=200;

void store(int a, int b) {
  if (a == 4) { // counting from 0
    row = -3;
    col++;
    dly -= 0;
    lc.setLed(lcnum,b, a,true);
    lc.setLed(lcnum,b, a-1, true);
  }
  else {
    // reset
    lcnum = 0;
    row = -3;
    col = 0;
    dly = 200;
    lc.clearDisplay(0);
    lc.clearDisplay(1);
  }

  if (col == 8) {
  //change lc display
    lcnum = 1;
    row = -3;
    col = 0;
    dly = 200;
  }

  if(col == 16) {
    //win
    lc.clearDisplay(0);
    lc.clearDisplay(1);
    lc.setLed(lcnum, 5, 5, true);
  }
}

void loop() {
  byte buttonState = digitalRead(buttonPin);
  
  for(row=0;row<9;row++){
    lc.setLed(lcnum,col, row-1,true);
    lc.setLed(lcnum,col, row,true);
    
    delay(dly);//sustabdo button laukima REIKIA FIX
    
    lc.setLed(lcnum,col, row-1,true);//KAM SITAS?
    lc.setLed(lcnum,col, row, true);

    lc.setLed(lcnum,col, row-1,false);
    lc.setLed(lcnum,col, row,false);
    
    byte buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      store(row, col);
    }
  }
}
//https://www.instructables.com/DOT-Matrix-8x8-Using-Arduino/
//https://gist.github.com/activeiz/9634195
