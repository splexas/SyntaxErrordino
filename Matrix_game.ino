//KARALIŠKASIS BOKŠTAS
//By Žilvinas S., Laurynas G., Natas M.

//Game settings
byte start_leds_quantity = 3;//recommended 3
int startspeed = 220;//recommended 220
byte speed_change_per_level = 8.5;//recommended 7-8 easy, 9-10 normal, 11-12 hard, 13+ imposible
byte brightness = 4;//recommended 1 (0-15)


#include <LedControl.h>
const byte DIN = 11;
const byte CS =12;
const byte CLK = 13;
const byte buttonPin = 5;

LedControl lc=LedControl(DIN, CLK, CS, 0);
void setup(){ 
  Serial.begin(9600);  
  
  pinMode(buttonPin, INPUT);

  lc.shutdown(0,false);
  lc.setIntensity(0,brightness);
  lc.clearDisplay(0);

  lc.shutdown(1,false);  
  lc.setIntensity(1,brightness);
  lc.clearDisplay(1);
}

byte address = 0;//8x8 matrix adrress
byte row = 0;//led row
byte col = 0;//led col
int dly = startspeed;//start speed
byte tail = start_leds_quantity;//start leds quantity
byte prevcol[] = {1, 1, 1, 1, 1, 1, 1, 1};//previous column state
byte releasted = true;//button is releasted

byte smile1[8] = {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C};
byte smile2[8] = {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
byte crown1[8] = {0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte crown2[8] = {0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00};
byte crown3[8] = {0xFF,0x00,0xFF,0x00,0x00,0x00,0x00,0x00};
byte crown4[8] = {0xFF,0xFF,0x00,0xFF,0x00,0x00,0x00,0x00};
byte crown5[8] = {0xBD,0xFF,0xFF,0x00,0xFF,0x00,0x00,0x00};
byte crown6[8] = {0x99,0xBD,0xFF,0xFF,0x00,0xFF,0x00,0x00};
byte crown7[8] = {0x00,0x99,0xBD,0xFF,0xFF,0x00,0xFF,0x00};
byte crown8[8] = {0x00,0x00,0x99,0xBD,0xFF,0xFF,0x00,0xFF};

void win(){//victory
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  for(byte adr = 0; adr <2; adr++){
    for(byte col = 0; col<8; col++){
      for(byte row = 0; row<8; row++){    
        lightleds(adr,col,row,true,1);
      delay(20);
      }
    }
  }
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  displayBytes(smile1, 0);
  displayBytes(crown1, 1);
  delay(300);
  displayBytes(crown2, 1);
  delay(300);
  displayBytes(crown3, 1);
  delay(300);
  displayBytes(crown4, 1);
  delay(300);
  displayBytes(crown5, 1);
  delay(300);
  displayBytes(crown6, 1);
  delay(300);
  displayBytes(crown7, 1);
  delay(300);
  displayBytes(crown8, 1);
  lc.clearDisplay(0);
  displayBytes(smile2, 0);
  delay(300);
  while(true){
    lc.clearDisplay(1);
    delay(100);
    displayBytes(crown8, 1);
    delay(300);
    byte buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH){
      reset();
      break;
    }
  }
}
void lose(){//gameover
  while(true){
    lightleds(address,col,row,true,tail);
    delay(150);
    lightleds(address,col,row,false,tail);
    delay(150);
    byte buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH){
      break;      
    }
  }
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
void displayBytes(byte character[], byte adr){//display saved bytes (custom picture)
  for(byte i=0;i<8;i++){
    lc.setRow(adr,i,character[7-i]);
  }
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
  return 0;
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
    lose();
    reset();
  }

  if (col == 8 and address == 0){//move to second matrix display
    address = 1;
    col = -1;
    nextlevel();
  }
  if(col == 8 and address == 1){
    win();
  }
}

void loop() {
  for(row=0;row<7+tail;row++){//kad judetu led'ai
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
