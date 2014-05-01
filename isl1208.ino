//device isl1208 address 0x6F
#include <Wire.h>
#include <avr/interrupt.h>
byte seconds, hours, minutes, date, months, years, statusReg;

void setup () {
  pinMode(2, INPUT);
  digitalWrite(2,HIGH);
  attachInterrupt(0, alarmTriggered, FALLING); 
  interrupts();
  delay(50);
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(0x6F); 
  Wire.write(0x07); //status register
  Wire.write(0x90);  //
  Wire.endTransmission();

  readStatusReg(statusReg);
  Serial.print(statusReg,HEX); //should be 144/0x90
  Serial.println("h - status register");

/*
  //alarm setup
  Wire.beginTransmission(0x6F); 
  Wire.write(0x08); //alarm register
  Wire.write(0xD0); 
  Wire.endTransmission();
  Wire.beginTransmission(0x6F); 
  Wire.write(0x0c); //seconds alarm register
  Wire.write(0xB0); //secs
  Wire.endTransmission(); 
  */
  
  /*
// set time
   Wire.beginTransmission(0x6F); 
   Wire.write(0x00); //seconds register
   Wire.write(decToBcd(30)); //secs
   Wire.write(decToBcd(41)); //min
   Wire.write(0xA3); //hour
   Wire.write(decToBcd(03)); //date
   Wire.write(decToBcd(8)); //month
   Wire.write(decToBcd(13)); //year
   Wire.endTransmission();
   Wire.beginTransmission(0x6f);
   Wire.write(0x07); //status reg addr
   Wire.endTransmission();
   Wire.requestFrom(0x6f,1); // now get the byte of data...
   byte statusReg=Wire.read();
   Serial.println(statusReg);
   */
}

void loop () {
  Wire.beginTransmission(0x6f);
  Wire.write(0x00); //status reg addr
  Wire.endTransmission();
  Wire.requestFrom(0x6f,6); // now get the byte of data...
  seconds=Wire.read();
  minutes=Wire.read();
  hours=Wire.read();
  date=Wire.read();
  months=Wire.read();
  years=Wire.read();

  hours=hours & B01111111; // remove mil time bit
  Serial.print(bcdToDec(hours));
  Serial.print(':');
  Serial.print(bcdToDec(minutes));
  Serial.print('.');
  Serial.print(bcdToDec(seconds));
  Serial.print(' ');
  Serial.print(bcdToDec(months));
  Serial.print('/');
  Serial.print(bcdToDec(date));
  Serial.print('/');
  Serial.println(bcdToDec(years));
  delay(1000);

}

byte readStatusReg(byte val){

  Wire.beginTransmission(0x6f);
  Wire.write(0x07); // read status reg to clr bits
  Wire.endTransmission();
  Wire.requestFrom(0x6f,1); // now get the byte of data...
  statusReg=Wire.read();
  return(statusReg);

}


byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

void alarmTriggered()
{
  Serial.println("** ALARM WENT OFF! **");

}
