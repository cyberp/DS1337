/**
* 
* Basic Tick Demo
*
* Set the time and date and ticks every minute.
* Print out current time and date to serial on every tick.
*
* INTB (DS1337) | SQW (DS3231) pin must be connected to pin INT0 (Uno = pin 2)
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
// tick
volatile boolean tick = false;

void setup() {
  // serial
  Serial.begin(115200);
  Serial.println("Tick Demo");
  
  // clear flags
  rtc.init();
  rtc.clearFlags();
  
  // set date 
  // 2014-10-20  
  rtc.setDate(14, 10, 20);
  
  // set time
  // 20:37 
  rtc.setTime(20, 37);
  
  // print current date and time
  printDateTime();
  Serial.println("...");
  
  // set tick every minute
  rtc.resetTick();
  rtc.setTickMode(DS1337_TICK_EVERY_MINUTE);
  
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
    // reset tick
    rtc.resetTick();
    tick = false;
    // print current date and time
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
