/**
* 
* Basic Alarm Demo
*
* Set the time and date and an alarm
*
* INTA (DS1337) | SQW (DS3231) pin must be connected to pin INT0 (Uno = pin 2)
*
*/

// include header 
#include <DS1337.h>
// use with DS3231:
// #include <DS3231.h>

// Wire.h must included always
#include <Wire.h>

// new RTC object
DS1337 rtc;
// use with DS3231:
// DS3231 rtc;

// Date object
Date dt;
// alarm
volatile boolean alarm = false;

void setup() {
	
  // serial
  Serial.begin(115200);
  Serial.println("Alarm Demo");
  
  // no flags, no ticks
  rtc.init();
  rtc.clearFlags();
  rtc.setTickMode(DS1337_NO_TICKS);
  
  // set date 
  // 2014-10-20
  rtc.setDate(14, 10, 20);
  
  // set time
  // 20:37 
  rtc.setTime(21, 32);
  
  // print current date and time
  printDateTime();
  
  // set alarm 2 minutes later
  // on 20th. 21:34
  rtc.clearAlarm();
  rtc.setAlarm(20, 21, 34, 0);
  // alarm on exact match 
  rtc.setAlarmMode(DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DATE);
  // enable
  rtc.enableAlarm();
  // print alarm
  printAlarm();
  Serial.println("....");
  
  // attach interrupt
  pinMode(2, INPUT);
  attachInterrupt(0, onAlarm, FALLING);
  
}

void loop() {
  // check if alarm occurs
  // don't use rtc over i2c in interrupt routine 
  // only outside
  if (alarm) {
    Serial.println("Alarm!");
    // clear alarm
    rtc.clearAlarm();
    alarm = false;
    // print current date and time
    printDateTime();
    Serial.println();
  }
}

// alarm interrupt routine
void onAlarm() {
  noInterrupts();
  alarm = true;
  interrupts();
}

// print current time and date
void printDateTime() {
  dt = rtc.getDate();
  Serial.print("Date: ");
  Serial.println(dt.getDateString());
  Serial.print("Time: ");
  Serial.println(dt.getTimeString());
}

// print current alarm
void printAlarm() {
  dt = rtc.getAlarm();
  Serial.print("Alarm: ");
  Serial.print(dt.getDay());
  Serial.print(" - ");
  Serial.println(dt.getTimeString());
}
