/**
* 
* Arlarm on the 13th. second
*
* Throws alarm on each 13th second of a minute.
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
  Serial.println("13th. Second Alarm");
  
  // no ticks, clear flags
  rtc.init();
  rtc.setTickMode(DS1337_NO_TICKS);
  rtc.clearFlags();

  // set date and time
  rtc.setDate(14, 10, 21);
  rtc.setTime(17, 13);

  // print current date and time
  printDateTime();
  
  // set alarm on 13th second of a minute
  // on 20th. 21:34
  rtc.setAlarm(21, 17, 13, 13);
  rtc.setAlarmMode(DS1337_ALARM_ON_SECOND);
  rtc.enableAlarm();
  
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

