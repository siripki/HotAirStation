#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte tempIcon[] = {
  B01110,
  B01010,
  B01010,
  B11011,
  B11111,
  B11111,
  B11111,
  B01110
};
byte setIcon[] = {
  B11111,
  B11111,
  B11000,
  B11111,
  B11111,
  B00011,
  B11111,
  B11111
};
byte fanIcon[] = {
  B00000,
  B11101,
  B01101,
  B11011,
  B11011,
  B10110,
  B10111,
  B00000
};
byte degIcon[] = {
  B00110,
  B01001,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000
};

#define pinThermo A0
#define pinHeater A1
#define pinFan A2
#define pinReed 12
#define fanOut 10
#define heaterOut 11

float vRef = 4.97;
//float adcResolution;
int temp,
heatPotValue,
fanPotValue,
setHeat,
setFan,
setHeatLCD,
setFanLCD;
String reedStat;

void setup() {
  //Serial.begin(9600);
  pinMode(pinThermo, INPUT);
  pinMode(pinHeater, INPUT);
  pinMode(pinFan, INPUT);
  pinMode(pinReed, INPUT_PULLUP);
  pinMode(fanOut, OUTPUT);
  pinMode(heaterOut, OUTPUT);

  lcd.begin();
  lcd.backlight();
  lcd.clear(); 
  lcd.setCursor(4,0);
  lcd.print("HOT AIR");
  lcd.setCursor(4,1);
  lcd.print("STATION");
  delay(1000);
  lcd.clear();

  lcd.createChar(0, tempIcon);
  lcd.createChar(1, setIcon);
  lcd.createChar(2, fanIcon);
  lcd.createChar(3, degIcon);
  
}

void loop() {
  //adcResolution = vRef/1024;
  temp = (analogRead(pinThermo))/(vRef/1024)*0.0041;
  
  heatPotValue = analogRead(pinHeater);
  //Serial.println(heatPotValue);
  setHeatLCD = map(heatPotValue, 0, 1023, 24, 525);
  
  fanPotValue = analogRead(pinFan);
  //Serial.println(fanPotValue);
  //setFanLCD = map(fanPotValue, 0, 1023, 0, 100);
  
  if(digitalRead(pinReed)==HIGH){
    reedStat="ON ";
    if(setHeatLCD == 24){
      if((temp-setHeatLCD)>=20){
        setHeat = 0;
        setFan = 255;
      }
      else{
        setHeat = 0;
        setFan = map(fanPotValue, 0, 1023, 0, 255);
      }
    }
    else if(temp < setHeatLCD && 24 < setHeatLCD <= 100){
      setHeat = 60;
      setFan = map(fanPotValue, 0, 1023, 0, 255);
    }
    else if(temp < setHeatLCD && 100 < setHeatLCD <= 150){
      setHeat = 80;
      setFan = map(fanPotValue, 0, 1023, 0, 255);
    }
    else if(temp < setHeatLCD && 150 < setHeatLCD <= 230){
      setHeat = 100;
      setFan = map(fanPotValue, 0, 1023, 0, 255);
    }
    else if(temp < setHeatLCD && 230 < setHeatLCD <= 300){
      setHeat = 125;
      setFan = map(fanPotValue, 0, 1023, 0, 255);
    }
    else if(temp < setHeatLCD && 300 < setHeatLCD <= 350){
      setHeat = 200;
      setFan = map(fanPotValue, 0, 1023, 0, 255);
    }
    else if(temp < setHeatLCD && 350 < setHeatLCD <= 525){
      setHeat = 255;
      setFan = map(fanPotValue, 0, 1023, 0, 255);
    }
    else if((temp-setHeatLCD)>=20){
      setHeat = 0;
      setFan = 255;
    }
    else if(temp >= setHeatLCD){
      setHeat = 0;
      setFan = map(fanPotValue, 0, 1023, 0, 255);
    }
  }
  if(digitalRead(pinReed)==LOW){
    reedStat="OFF";
    if(temp >= 100){
      setHeat = 0;
      setFan = 128;
    }
    else {
      setHeat = 0;
      setFan = 0;
    }
  }
  setFanLCD = map(setFan, 0, 255, 0, 100);
  analogWrite(heaterOut, setHeat);
  analogWrite(fanOut, setFan);
  
  display();
}
void display() {
  unsigned long dispTimer;
  if(millis() - dispTimer >= 100){
    //
    lcd.setCursor(0,0);
    lcd.write(byte(0));
    lcd.setCursor(0,1);
    lcd.write(byte(1));
    lcd.setCursor(9,0);
    lcd.write(byte(2));
    lcd.setCursor(10,0);
    lcd.write(byte(2));
    lcd.setCursor(5,0);
    lcd.write(byte(3));
    lcd.setCursor(6,0);
    lcd.print("C");
    lcd.setCursor(11,1);
    lcd.print("%");

    lcd.setCursor(2,0);
    lcd.print("   ");
    lcd.setCursor(2,0);
    lcd.print(temp);

    lcd.setCursor(2,1);
    lcd.print("   ");
    lcd.setCursor(2,1);
    lcd.print(setHeatLCD);

    lcd.setCursor(8,1);
    lcd.print("   ");
    lcd.setCursor(8,1);
    lcd.print(setFanLCD);
    
    lcd.setCursor(13,0);
    lcd.print(reedStat);

    dispTimer=millis();
  }
}
