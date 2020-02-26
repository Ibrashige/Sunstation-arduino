#define touchSw 14
#include <LedControl.h>
#include <RTClib.h>

int val = 0; // variable for reading the pin status
int counter = 0;
int currentState = 0;
int previousState = 0;
//char setCounter;
double hold;

LedControl lc = LedControl(4, 2, 15, 1);
RTC_DS1307 rtc;

void setup(){
  rtc.begin();
  lc.shutdown(0, false);
  lc.setIntensity(0, 8); // display brightness
  lc.clearDisplay(0); // erase display
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //set clock as computer clock
  //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  pinMode(touchSw,INPUT);
  Serial.begin(9600);

}

void loop(){
  boolean touchState;
  val = digitalRead(touchSw);
  touchState = digitalRead(touchSw);
// if(touchState == HIGH)
// {
if(val == HIGH){
  if(val == HIGH){
      currentState = 1;
  }
  else if (val != HIGH){
      currentState = 0;
  }
  else if(currentState != previousState){
      if(currentState == 1){
        counter = counter + 1;
        Serial.println(counter);
      }
  }
  previousState = currentState;
  delay(250);
     
  hold = millis();
  while ((hold + 5000) > millis()){
     setCounter(counter);
  }
}
  else{
    hold = millis();
  while ((hold + 1000) > millis()){
      DateTime now = rtc.now();
      setCol1(now.hour());
      setCol2(now.minute());
      setCol3(now.second());
      }
    }
}

//========================================================================================
void setCol1 (byte jam){
  String strBuffer = Konversi(jam);
  byte pjg = strBuffer.length() + 1;
  char val[pjg];
  strBuffer.toCharArray(val, pjg);
  lc.setChar (0, 3, val[0], false);
  lc.setChar (0, 2, val[1], false);
}

//========================================================================================
void setCol2 (byte mnt){
  String strBuffer = Konversi(mnt);
  byte pjg = strBuffer.length() + 1;
  char val[pjg];
  strBuffer.toCharArray(val, pjg);
  lc.setChar (0, 1, val[0], false);
  lc.setChar (0, 0, val[1], false);
}

void setCol3 (byte dtk){
  String strBuffer = Konversi(dtk);
  byte pjg = strBuffer.length() + 1;
  char val[pjg];
  strBuffer.toCharArray(val, pjg);
  lc.setChar (0, 6, val[0], false);
  lc.setChar (0, 7, val[1], false);
}

void setCounter (byte kim){
  String strBuffer = Konversi(kim);
  byte pjg = strBuffer.length() + 1;
  char val[pjg];
  strBuffer.toCharArray(val, pjg);
  lc.setChar (0, 3, val[0], false);
  lc.setChar (0, 2, val[1], false);
  lc.setChar (0, 1, val[0], false);
  lc.setChar (0, 0, val[1], false);
}

String Konversi(int nilai){
  if (nilai < 10) 
  {
     return "0" + String(nilai);
  }
  else 
  {
    return String(nilai);
  }
}
