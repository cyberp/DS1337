/**
* 
* Basic Time/Date Demo
*
* Set the time and date in setup function.
* In loop get the current date and time, print out to serial connection, and delay 3 seconds ...
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

void setup() {
  // serial
  Serial.begin(115200);
  Serial.println("Time and Date Demo");
  
  // set date 
  // 2014-10-20 
  rtc.init();
  rtc.setDate(14, 10, 20);
  
  // set time
  // 20:23 
  rtc.setTime(20, 23);
}

void loop() {
  // get current time and date
  dt = rtc.getDate();
  Serial.print("Date: ");
  Serial.println(dt.getDateString());
  Serial.print("Time: ");
  Serial.println(dt.getTimeString());
  
  // delay 3 sec
  delay(3000);
}
