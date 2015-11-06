/**
*
* DS1337 Interactive Demo
* 
* Demo sketch for using the DS1337/DS3231 library.
*
*/
// include header 
#include <DS1337.h>

// Wire.h must included always
#include <Wire.h>

// new RTC object
DS1337 rtc;

// helper
volatile boolean tick = false;
volatile boolean alarm = false;
String input = "";

/**
* Setup 
*/
void setup() {
  // clear alarm/ticks flags#
  rtc.init();
  rtc.clearAlarm();
  rtc.resetTick();
  
  // serial connection
  Serial.begin(115200);
  Serial.println("DS1337 Demo");
  Serial.println("-----------");
  Serial.println();
  
  // initial info and help
  showInfo();
  showHelp();
  
  // interrupts DS1337
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(0, onTick, FALLING); 
  attachInterrupt(1, onAlarm, FALLING); 
}

/**
* Main loop 
*/
void loop() {
    if (alarm) {
      Serial.println("ALARM!");
      Serial.println(rtc.getDate().getTimeString());
      rtc.clearAlarm();
      alarm = false;
    }
    if (tick) {
      Serial.println("TICK!");
      Serial.println(rtc.getDate().getTimeString());
      rtc.resetTick();
      tick = false;
    }
}

/**
* Serial event handling 
*/
void serialEvent() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c!='\r') {
      if (input.length()<20 && c!='\n') {
        input = input + c;
      }
    }
    // analyze input
    else if (input.length()>0) {
      // first char is command
      char cmd = input.charAt(0);
      boolean updated = false;
      String value = "";
      if (input.length()>1) {
        value = input.substring(1);        
      }
      switch (cmd) {
        case 'm': switchTickMode(); break;
        case 'n': switchAlarmMode(); break;
        case 'q': rtc.toggleAlarm(); updated = true; break;
        case 'c': rtc.clearAlarm(); rtc.resetTick(); updated = true; break; 
        case 'h': showHelp(); break;
        case 'i': showInfo(); break;
        case 'r': showRegister(); break;
        case 'x': toggleClock(); break;
        case 'o': rtc.clearOSF(); updated = true; break;
        case 'd': rtc.setDate(value); updated = true; break;
        case 't': rtc.setTime(value); updated = true; break;
        case 'a': rtc.setAlarm(value); updated = true; break;
      }
      if (updated)
        showInfo();
      updated = false;
      input = "";
    }
  }
}

/**
* Show help 
*/
void showHelp() {
  Serial.println(F("HELP"));
  Serial.println(F("----"));
  Serial.println(F("dyy-mm-dd ............... set date with form yy-mm-dd"));
  Serial.println(F("thh:mm ..................... set time with form hh:mm"));
  Serial.println(F("a[dd.]hh:mm ... set alarm with form dd.hh:mm or hh:mm"));
  Serial.println(F("m .......................... cycle through tick modes"));
  Serial.println(F("n ......................... cycle through alarm modes"));
  Serial.println(F("q .............................. enable/disable alarm"));
  Serial.println(F("c ........................... clear active alarm/tick"));
  Serial.println(F("h ................................... print this help"));
  Serial.println(F("i ........................... show status information"));
  Serial.println(F("r .......................... show values of registers"));
  Serial.println(F("x ......................... enable/disable oscillator"));
  Serial.println(F("o .................................... clear OSF flag"));
  Serial.println();
}

/**
* Info 
*/
void showInfo() {
  Serial.println("INFO");
  Serial.println("----");
 
  // clock
  Serial.print("CLOCK: ");
  // time
  Serial.print(rtc.getDate().getTimeString());
  Serial.print(" - ");
  // date
  Serial.print(rtc.getDate().getDateString());
  // oscillator
  if (!rtc.isRunning())
    Serial.print(" STOPPED");
  else
    Serial.print(" RUNNING");
  if (rtc.hasStopped()) 
    Serial.print(" HAS STOPPED");
  Serial.println();
  
  // alarm
  Serial.print("ALARM: ");
  Serial.print(rtc.getAlarm().getTimeString());
  Serial.print("@");
  Serial.print(rtc.getAlarm().getDay());
  if (rtc.isAlarmEnabled()) 
    Serial.print(" ENABLED");
  else
    Serial.print(" DISABLED");
  if (rtc.isAlarmActive())
    Serial.print(" ACTIVE");
  switch (rtc.getAlarmMode()) {
    case DS1337_ALARM_EVERY_SECOND: Serial.print(" - ON EVERY SECOND"); break;
    case DS1337_ALARM_ON_SECOND: Serial.print(" - ON SECOND"); break;
    case DS1337_ALARM_ON_SECOND_MINUTE: Serial.print(" - ON SECOND/MINUTE"); break;
    case DS1337_ALARM_ON_SECOND_MINUTE_HOUR: Serial.print(" - ON SECOND/MINUTE/HOUR"); break;
    case DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DATE: Serial.print(" - ON SECOND/MINUTE/HOUR/DATE"); break;
    case DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DAY: Serial.print(" - ON SECOND/SECOND/MINUTE/HOUR/DAY"); break;
    default: Serial.print(" - UNKNOWN");  
  }
  Serial.println();
  
  // tick mode
  int tickMode = rtc.getTickMode();
  Serial.print("TICKS: ");
  switch (tickMode) {
    case DS1337_NO_TICKS: Serial.println("OFF"); break;
    case DS1337_TICK_EVERY_SECOND: Serial.println("EVERY SECOND"); break;
    case DS1337_TICK_EVERY_MINUTE: Serial.println("EVERY MINUTE"); break;
    case DS1337_TICK_EVERY_HOUR: Serial.println("EVERY HOUR"); break;
    default: Serial.println("UNKNOWN");
  }
  Serial.println();
}


/**
* Show Register
*/
void showRegister() {
  for (int i=0; i<DS1337_REGISTERS; i++) {
    Serial.print("R 0x");
    Serial.print(i, HEX);
    Serial.print(": 0x");
    Serial.print(rtc.getRegister(i), HEX);
    Serial.print(" ");
    Serial.print(rtc.getRegister(i), BIN);
    Serial.println("b");
  }
}

/**
* Cycle through different tick modes 
*/
void switchTickMode() {
  int tickMode = rtc.getTickMode();
  tickMode = (tickMode+1) % 4;
  rtc.setTickMode(tickMode);
  showInfo();
}

/**
* Cycle through different tick modes 
*/
void switchAlarmMode() {
  int alarmMode = rtc.getAlarmMode();
  alarmMode = (alarmMode+1) % 6;
  rtc.setAlarmMode(alarmMode);
  showInfo();
}

/**
* Enable/Disable Clock
*/
void toggleClock() {
  if (rtc.isRunning()) {
    rtc.stop();
  }
  else {
    rtc.start();
  }
  showInfo();
}

/**
* Alarm Interrupt handler 
*/ 
void onAlarm() {
  noInterrupts();
  alarm = true;
  interrupts();
}

/**
* Tick interrupt handler
*/
void onTick() {
  noInterrupts();
  tick = true;
  interrupts();
}

