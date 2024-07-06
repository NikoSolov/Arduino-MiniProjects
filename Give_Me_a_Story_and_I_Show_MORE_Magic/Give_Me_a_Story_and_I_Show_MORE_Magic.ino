#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
String list[4], dir ="/", file, str; int ch=0; //Для выбора
String d, s; int spd(0), m(5), l(0), n(0);//Для обработки текста
/////////////////////////////////////////
int NameChoice(char* names, ... ){
va_list argptr;
va_start (argptr, names);
lcd.setCursor(1, 0);
lcd.print(names);
int m(1);
char * message;
while(message){  
  lcd.setCursor(1, m);
  message = va_arg( argptr, char *);
  lcd.print( message );
  m+=1;}
  va_end(argptr);
return m;
}
/////////////////////////////////////////
int Choice(int m){//Метод выбора. Входные данные: Максимальное значение выбора. Выходные данные: Номер выбор [0,  максимальное-1]
int pos(0);
while(digitalRead(5)){}
    while(!digitalRead(5)){
      if (digitalRead(5) or digitalRead(4)){
         lcd.setCursor(0,pos);
         lcd.print(" ");
         if(digitalRead(3)){while(!digitalRead(4)){} pos+=1; while(digitalRead(3) and digitalRead(4)){}}
         else if(digitalRead(4)){while(!digitalRead(3)){} pos-=1; while(digitalRead(3) and digitalRead(4)){}}
         if (pos>m-1){pos=0;}
         else if (pos<0){pos=m-1;}}         
         lcd.setCursor(0,pos);
         lcd.print(">");}
return pos;
}
/////////////////////////////////////////////////////////
int NameFile(String root){//Печать названий файлов. Входные данные: Коренное положение в SD-карте. Выходные данные: максимальное значение файлов
    int m(1);
    //for(int i; i<4; i++){list[i]=" ";}
    lcd.setCursor(1,0);
    lcd.print("..");
    File dir = SD.open(root);//открываем каталог
    while (true) {
    File files =  dir.openNextFile();//открываем следующий файл
    if (!files) {break;}//проверка на существования такого файла
    lcd.setCursor(1,m);//Настраиваем на вывод курсор   
    lcd.print(files.name());
    list[m]=files.name();
    m+=1;}
    return m;
}
//////////////////////////////////////////////////
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(2, OUTPUT);//Пьезоизлучателя
  pinMode(3, INPUT);//                        Поворот налево
  pinMode(4, INPUT);//Инициализация энкодера: Поворот направо
  pinMode(5, INPUT);//                        Кнопка
//Приветствие  
  lcd.setCursor(5,1);
  tone(2, 440, 100);
  lcd.print("WELCOME!!!");
  delay(1000);
  lcd.clear();  
  delay(1000);
//
  n=Choice(NameChoice("Demo", "SD-card", NULL)-1);
  lcd.clear();
if (n==1){  
  while(true){
  while(!SD.begin(10)){lcd.print("Enter a SD-card"); lcd.setCursor(0,0);}
  lcd.clear();

  lcd.print(dir);
  delay(3000);  
  
  ch=Choice(NameFile(dir));
  File Data = SD.open(list[ch]);
  
  lcd.clear();
  file=Data.name();
  if (ch==0){lcd.print("true1");dir.replace(list[ch]," ");} 
  else{
    if (Data.isDirectory()) {lcd.print("true");dir+=file;}
    else{Data = SD.open(dir+"/"+list[ch]); while (Data.available()) {str=Data.readString();}  Data.close(); break;}
    }
  }
}
if (n==0){str="HEllo, world!!!";}
}
////////////////////////////////////////////////////////////////
//Hi from GNU nano 3.2
void loop() { 

for (int i = 0; i < str.length(); i++) {
    
/**/if (str[i] == '%') {lcd.clear();}

/**/else if (str[i] == '/') {while (digitalRead(5) != 1) {}}

/**/else if (str[i] == '^') {i += 1; d = str[i]; i += 1;
      while (str[i] != '^') {d += str[i];i += 1;} 
      delay(d.toInt());}

/**/else if (str[i] == '<') {i += 1; s = str[i]; i += 1;
      while (str[i] != '>') {s += str[i]; i += 1;} 
      spd = s.toInt();}

/**/else if (str[i] == '#') {lcd.setCursor(6, 2); lcd.print("YES"); lcd.setCursor(11, 2); lcd.print("NO");
       l = 0; n=5;
      while (digitalRead(5) != 1) {
        lcd.setCursor(n, 2); lcd.print(">");
        if (digitalRead(3) == 1) {lcd.setCursor(n, 2); lcd.print(" ");
          digitalWrite(2, HIGH); delay(10); digitalWrite(2, LOW);
          if (n == 5) {n = 10; l = 1;}
          else if (n == 10) {n = 5; l = 0;}
          lcd.setCursor(n, 2);
          lcd.print(">");
          while (digitalRead(3) != 0) {}}}
      lcd.setCursor(0, 2); lcd.print("                    "); i += 1;
      if (l == 0) {i += 1;
        while (str[i] != '1') {lcd.print(str[i]);
          if (str[i] != ' ') {digitalWrite(2, HIGH);}
          delay(10);digitalWrite(2, LOW);delay(spd);i += 1;}
        while (str[i] != '#') {i += 1;}}
      else if (l == 1) {i += 1;
        while (str[i] != '1') {i += 1;}
        i += 1;
        while (str[i] != '#') {lcd.print(str[i]);
          if (str[i] != ' ') {digitalWrite(2, HIGH);}
          delay(10); digitalWrite(2, LOW); delay(spd); i += 1;}}}

/**/else {lcd.print(str[i]);
      if (str[i] != ' ') {digitalWrite(2, HIGH);}
      delay(10); digitalWrite(2, LOW); delay(spd);}
}
}
