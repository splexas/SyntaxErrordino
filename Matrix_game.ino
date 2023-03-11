//By Žilvinas, Laurynas, Natas

//Game settings
byte start_leds_quantity = 4;//recommended 3
int startspeed = 200;//recommended 200
byte speed_change_per_level = 20;//recommended 20




#include <LedControl.h>
const byte DIN = 11;
const byte CS = 7;
const byte CLK = 13;
const byte buttonPin = 5;

LedControl lc=LedControl(DIN, CLK, CS,0);
void setup(){
  Serial.begin(9600);  
  
  pinMode(buttonPin, INPUT);

  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);

  lc.shutdown(1,false);  
  lc.setIntensity(1,0);
  lc.clearDisplay(1);
}




byte address = 0;//8x8 matrix adrress
byte row = 0;//led row
byte col = 0;//led col
int dly = startspeed;//start speed
byte tail = start_leds_quantity;//start_leds_quantity
byte prevcol[] = {1, 1, 1, 1, 1, 1, 1, 1};//previous column state
byte releasted = true;//button is releasted

void win(){//victory
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.setLed(address, 5, 5, true);
}
void reset(){//reset
  address = 0;
  row = -1;
  col = 0;
  dly = startspeed;
  tail = start_leds_quantity;
  for (byte i = 0; i < 8; i++){
    prevcol[i] = 1;
  }
  lc.clearDisplay(0);
  lc.clearDisplay(1);
}
void lightleds(byte address, byte x, byte y, byte status, byte tail){//light certain leds
  for(byte i = 0; i<tail; i++){
    lc.setLed(address, x, y-i, status);
  }
}
void nextlevel(){//next level
  row = 0;
  col++;
  dly -= speed_change_per_level;
}
byte getledunder(byte rownum){//checks if led under current row is on
  for (byte i = 0; i < 8; i++){
    if(i == rownum){
      return prevcol[i];
    }
  }
}
void updateprevcol(byte suitablerow){//updates previous column state
  for (byte i = 0; i < 8; i++){
    if(i >= suitablerow - (tail-1) and i <= suitablerow){
      prevcol[i] = 1;
    }
    else{
      prevcol[i] = 0;
    }        
  }  
}


void store(){//store leds
  byte cropedtail = 0;
  byte onetime = true;
  byte srow = row;
  lightleds(address,col,row,false,tail);
  
  for (byte i = 0; i < tail; i++){//kartoja tiek kartu kokio didumo yra tail
    if (getledunder(row-i) == 1){ 
      lightleds(address,col,row-i,true,1);
      cropedtail++;
      if(onetime == true){
        onetime = false;
        srow = row-i;
      }
    }
  }
  if (cropedtail >= 1){//jei bent vienas led atitiko
    tail = cropedtail;
    updateprevcol(srow);//fixxx
    nextlevel();
  } 
  else{
    reset();
  }

  if (col == 8) {//move to second matrix display
    address = 1;
    nextlevel();
  }
  if(col == 16){
    win();
  }
}

void loop() {
  for(row=0;row<9;row++){//kad judetu led'ai
    lightleds(address,col,row,true,tail);

    for(byte i=0;i<10;i++){//visa delay padalija i n daliu ir tarp ju tikrina ar paspaustas mygtukas
      delay(dly/10);//todel kad greitai paspaudus ir atleidus mygtuka ji vistiek aptiktu
      byte buttonState = digitalRead(buttonPin);//nes funkcija delay() "sustabdo" code flow
      if (buttonState == HIGH){
        if(releasted == true){//patikrina kad per ilgai nuspaustas mygtukas neturetu itakos
          store();
          releasted = false;
        }
      }
      else{
        releasted = true;
      }
    }

    lightleds(address,col,row,false,tail);//isjungia reikiamus led kad kiti uzsidegtu ir gautusi animacija
    
  }
}
// https://www.instructables.com/DOT-Matrix-8x8-Using-Arduino/
//EEPROM   kad storint be elektros data
