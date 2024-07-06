int latchPin = 3, clockPin = 4, dataPin = 2, piezo=7, button1=5, button2=6;
bool CLK=false, But1=false, But2=false, But3=false;
int second(0), minute(0), hour(0);
unsigned long secondMillis, currentMillis, clkMillis;

  enum status {
    CLOCK,
    SetMinute,
    SetHour,
    TEXT};
  status state;
void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  tone(9, 440, 1000);
 secondMillis = millis();
 Serial.begin(9600);
 state = CLOCK;
}
              //abcdefgd
 byte num[]={ 0b00000011,
              0b10011111,
              0b00100101,
              0b00001101, 
              0b10011001,
              0b01001001,
              0b01000001,
              0b00011111, 
              0b00000001,
              0b00001001};
 
char disp[4]={};
int n=0;
int c=0;   

void loop() {   //abcdefgd

  if (digitalRead(5)==0 and But1==false){
    if (state==CLOCK){state=SetHour;}
    else if (state==SetHour){state=SetMinute;}
    else if (state==SetMinute){state=CLOCK; second=0;} 
    But1=true;
  }
  if (digitalRead(5)==1 and But1==true){But1=false;}

  if (state==CLOCK or state==SetMinute or state==SetHour){
    disp[0]=num[minute%10];
    disp[1]=num[minute/10];
    disp[2]=num[hour%10];
    disp[3]=num[hour/10];
    if (state==CLOCK and CLK==true){disp[2]=disp[2]-1;}
    if (state==SetMinute){
      disp[0]=num[minute%10]-1;
      disp[1]=num[minute/10]-1;
    }
    if (state==SetHour){
      disp[2]=num[hour%10]-1;
      disp[3]=num[hour/10]-1;
    }
  }
  if (state==SetMinute){
    if (digitalRead(6)==0 and But2==false){
      minute--;
      if (minute<0){minute=59;}
      But2=true;
    }
    if (digitalRead(6)==1 and But2==true){But2=false;}
    
    if (digitalRead(7)==0 and But3==false){
      minute++;    
      if (minute>59){minute=0;}
      But3=true;
    }
    if (digitalRead(7)==1 and But3==true){But3=false;}
    
  }

  if (state==SetHour){
    if (digitalRead(6)==0 and But2==false){
      hour--;
      if (hour<0){hour=23;}
      But2=true;
    }
    if (digitalRead(6)==1 and But2==true){But2=false;}
    
    if (digitalRead(7)==0 and But3==false){
      hour++;    
      if (hour>23){hour=0;}
      But3=true;
    }
    if (digitalRead(7)==1 and But3==true){But3=false;}
  }

  
  
  /* Time */
  currentMillis = millis();  //get the current time
  if (currentMillis - secondMillis >= 1000)  //test whether the period has elapsed
  {
    second+=1;
    if (second>59){minute++; second=0;}
    if (minute>59){hour++; minute=0;}
    if (hour>23){hour=0;}
    secondMillis=currentMillis;
  }
  if (currentMillis - clkMillis >= 500){
    CLK=not(CLK);
    clkMillis=currentMillis;
  }
  /* Display Update */
  for (int i(0); i<4;i++){shiftOut(dataPin, clockPin, MSBFIRST, disp[i]);}
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);

}