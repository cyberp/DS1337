/**

DS1337.h

Copyright by Christian Paul, 2014

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

 */
#ifndef DS1337_h
#define DS1337_h

// includes
#include <Arduino.h>

// byte
typedef uint8_t byte;


// constants
#define SECONDS_PER_MINUTE    UL60
#define SECONDS_PER_HOUR    UL3600
#define SECONDS_PER_DAY    UL86400

// DS1337 I2C BUS ID
#define DS1337_ID  B1101000

// DS1337 registers
#define DS1337_REGISTERS   		16
#define DS1337_REGISTERS_DATE    7
#define DS1337_REGISTERS_A1      4
#define DS1337_REGISTERS_A2      3
#define DS1337_REGISTERS_STATUS  2
#define DS1337_SECONDS     0x00
#define DS1337_MINUTES     0x01
#define DS1337_HOUR        0x02
#define DS1337_DAY_OF_WEEK 0x03
#define DS1337_DAY         0x04
#define DS1337_MONTH       0x05
#define DS1337_YEAR        0x06
#define DS1337_A1_SECONDS  0x07
#define DS1337_A1_MINUTES  0x08
#define DS1337_A1_HOUR     0x09
#define DS1337_A1_DAY      0x0A
#define DS1337_A2_MINUTES  0x0B
#define DS1337_A2_HOUR     0x0C
#define DS1337_A2_DAY      0x0D
#define DS1337_CONTROL     0x0E
#define DS1337_STATUS      0x0F

// DS1337 control register flags
#define DS1337_A1IE 	0x00
#define DS1337_A2IE 	0x01
#define DS1337_INTCN	0x02
#define DS1337_RS1		0x03
#define DS1337_RS2		0x04
#define DS1337_EOSC		0x07

// DS1337 status register flags
#define DS1337_A1F 		0x00
#define DS1337_A2F 		0x01
#define DS1337_OSF		0x07

// DS1337 tick modes
#define DS1337_TICK_UNKNOWN			0xFF
#define DS1337_NO_TICKS				0x00
#define DS1337_TICK_EVERY_SECOND  	0x01
#define DS1337_TICK_EVERY_MINUTE  	0x02
#define DS1337_TICK_EVERY_HOUR  	0x03

// DS1337 alarm modes
#define DS1337_ALARM_EVERY_SECOND 				0x00
#define DS1337_ALARM_ON_SECOND 					0x01
#define DS1337_ALARM_ON_SECOND_MINUTE 			0x02
#define DS1337_ALARM_ON_SECOND_MINUTE_HOUR 		0x03
#define DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DATE	0x04
#define DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DAY  0x05
#define DS1337_ALARM_UNKNOWN					0xFF

// DS1337 alarm mode register
#define DS1337_A1M1		7
#define DS1337_A1M2		7
#define DS1337_A1M3		7
#define DS1337_A1M4		7
#define DS1337_A1DYDT	6
#define DS1337_A2M2		7
#define DS1337_A2M3		7
#define DS1337_A2M4		7
#define DS1337_A2DYDT	6

// Helpers
#define	T2000UTC 	946684800UL


// class definition of Date object
class Date {
	public:
		Date();
		Date(int hour, int minutes, int seconds);
		Date(int year, int month, int day, int hour, int minutes, int seconds);
		int getSeconds();
		int getMinutes();
		int getHour();
		int getDay();
		int getMonth();
		int getYear();
		void setSeconds(int seconds);
		void setMinutes(int minutes);
		void setHour(int hour);
		void setDay(int day);
		void setMonth(int month);
		void setYear(int year);
		void setTime(int hour, int minutes, int seconds);
		void setDate(int year, int month, int day);
		String getTimeString();
		String getDateString();
		unsigned long getTimestamp();
	private:
		int _seconds;
		int _minutes;
		int _hour;
		int _day;
		int _month;
		int _year;
};

// class definition of DS1337 RTC
class DS1337 {
	public:
		DS1337();
		void init();
		void setTime(int hour, int minutes);
		void setTime(int hour, int minutes, int seconds);
		void setTime(String time);
		void setDate(int year, int month, int day);
		void setDate(String date);
		void setDateTime(int year, int month, int day, int hour, int minutes, int seconds);
		void setDateTime(String date);
		void setDateTime(unsigned long timestamp);
		void start();
		void stop();
		boolean isRunning();
		void setDate(Date date);
		Date getDate();
		void setAlarm(int day, int hour, int minutes, int seconds);
		void setAlarm(int hour, int minutes, int seconds);
		void setAlarm(Date date);
		void setAlarm(String date);
		void snooze(int minutes);
		void saveAlarm();
		void restoreAlarm();
		Date getAlarm();
		void enableAlarm();
		void disableAlarm();
		void clearAlarm();
		void toggleAlarm();
		boolean isAlarmEnabled();
		boolean isAlarmActive();
		int getRegister(int i);
		boolean isTickActive();
		void setTickMode(int tickMode);
		int getTickMode();
		void resetTick();
		int getDayOfWeek();
		void setDayOfWeek(int day);
		boolean hasStopped();
		void clearOSF();
		void clearFlags();
		void setAlarmMode(int alarmMode);
		int getAlarmMode();
		static void getTime(unsigned long timestamp, int &year, int &month, int &day, int &hour, int &minute, int &second);
		static unsigned long getTimestamp(int year, int month, int day, int hour, int minute, int second);
		unsigned long getTimestamp();
	protected:
		void readStatus();
		void writeStatus();
		byte _register[DS1337_REGISTERS];
		int _tickMode;
		void readAlarm2();
		void writeAlarm2();
		void clear();
		void read(int startRegister, int countRegister);
	private:
		void readDate();
		void readAlarm1();
		void write(int startRegister, int countRegister);
		void writeDate();
		void writeAlarm1();
		Date _date;
		Date _alarm;
		Date _savedAlarm;
		int _alarmMode;
};

#endif
