void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i(2); i<14; i++){pinMode(i, OUTPUT);}
  digitalWrite(3, HIGH); //bc2
  YM_ADDR(7);
  YM_SENT(0x80);
  delay(2000);
  YM_ADDR(0xf);
  delay(2000);
  YM_SENT(0x55);
  delay(2000);
  YM_ADDR(0xe);

}

void loop() {
//  Serial.println(YM_read())
  // put your main code here, to run repeatedly:
}


void YM_ADDR(int data) {
  digitalWrite(2, LOW); //bdir
  digitalWrite(4, LOW); //bc1
  for (int i(5); i<14; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, data%2);
    data/=2; 
  }
  digitalWrite(4, HIGH); //bc1
  digitalWrite(2, HIGH); //bdir
  delay(2000);
  digitalWrite(2, LOW); //bdir
  digitalWrite(4, LOW); //bc1
  for (int i(5); i<14; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, 0); 
  }


}

void YM_SENT(int data) {
  digitalWrite(2, LOW); //bdir
  digitalWrite(4, LOW); //bc1
  for (int i(5); i<14; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, data%2);
    data/=2; 
  }
  digitalWrite(2, HIGH); //bdir
  digitalWrite(2, LOW); //bdir
  delay(2000);
  for (int i(5); i<14; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }

}


int YM_read() {
  int data=0;
  digitalWrite(2, LOW); //bdir
  digitalWrite(4, LOW); //bc1
  for (int i(5); i<14; i++){
    pinMode(i, INPUT);
    data+=digitalRead(i);
    data*=2; 
  }
  data/=2;
  return data;
}
