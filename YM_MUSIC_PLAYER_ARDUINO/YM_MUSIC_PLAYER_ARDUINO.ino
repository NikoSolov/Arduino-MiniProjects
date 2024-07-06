byte digits[]={0b00000011,0b10011111,0b00100101, 0b00001101, 0b10011001, 0b01001001,0b01000001, 0b00011111, 0b00000001, 0b00001001, 0b00010001, 0b11000001, 0b01100011, 0b10000101, 0b01100001, 0b01110001};
int notes[]={3821, 3607, 3404, 3213, 3033, 2862, 2702, 2550, 2407, 2272, 2144, 2024};
#include <LiquidCrystal_I2C.h>

#define shift_data 5
#define shift_clk 6
#define bc1 8
#define bdir 7

#include <SPI.h>
#include <SD.h>
File myFile;
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight(); 
  // put your setup code here, to run once:

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  for(int i(5); i<9; i++){pinMode(i, OUTPUT);}
  
  sent_ym(0xfe, 7);
  sent_ym(15, 8);
  
  sent_ym(2, 1);
//  while (true){}
}
  int j=0, n=1, m=1;
void loop() {
  lcd.setCursor(0,0);
  lcd.print(m);
  lcd.print(" ");
  lcd.print(j);
  lcd.print("  ");
  
  sent_ym((notes[j]/n)%256,0);
  sent_ym((notes[j]/n)/256,1);


  j++;
  if (j>11){j=0; n*=2; m+=1;}
  if (m>8){n=1;m=1;}
  delay(250); 

}


void sent_ym(byte data, byte reg){
shiftOut(shift_data, shift_clk, MSBFIRST, reg);
digitalWrite(bc1, HIGH);
digitalWrite(bdir, HIGH);
digitalWrite(bdir, LOW);
digitalWrite(bc1, LOW);
shiftOut(shift_data, shift_clk, MSBFIRST, data);
digitalWrite(bdir, HIGH);
digitalWrite(bdir, LOW);
}
