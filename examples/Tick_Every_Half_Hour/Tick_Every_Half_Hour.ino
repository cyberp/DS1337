/**
* 
* Tick every half hour
*
* SQW (DS3231) pin must be connected to pin INT0 (Uno = pin 2)
*
*/

// include header 
#include <DS3231.h>

// Wire.h must included always
#include <Wire.h>

// new RTC object
DS3231 rtc;

// Date object
Date dt;
// tick
volatile boolean tick = false;

void setup() {
  // serial
  Serial.begin(115200);
  Serial.println("Tick Every Half Hour Demo");
  Serial.println();
  
  // clear flags
  rtc.clearFlags();
  Serial.println("Now");
  printDateTime();
  Serial.println();
  
  // set tick every hour
  rtc.init();
  rtc.resetTick();
  rtc.setTickMode(DS1337_TICK_EVERY_HOUR);
  
  // alarm on 30th. minute
  rtc.setAlarm(0, 30, 0);
  rtc.setAlarmMode(DS1337_ALARM_ON_SECOND_MINUTE);
  rtc.enableAlarm();
  
  // attach interrupt
  pinMode(2, INPUT);
  attachInterrupt(0, onTick, FALLING);
  
}

void loop() {
  // check if tick occurs
  // don't use rtc over i2c in interrupt routine 
  // only outside
  if (tick) {
    Serial.println("Tick Tack");
    rtc.clearFlags();
    tick = false;
    printDateTime();
    Serial.println();
  }
}

// Tick interrupt routine
void onTick() {
  noInterrupts();
  tick = true;
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
