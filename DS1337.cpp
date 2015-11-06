/**

DS1337.cpp

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
#include "DS1337.h"
#include "Wire.h"

/**
 * Constructor of class Date
 */
Date::Date() {
	setDate(1, 1, 0);
	setTime(0, 0, 0);
}

/**
 * Constructor of class Date with given time
 */
Date::Date(int hour, int minutes, int seconds) {
	setDate(1, 1, 0);
	setTime(hour,  minutes, seconds);
}

/**
 * Constructor of class Date with given date and time
 */
Date::Date(int year, int month, int day, int hour, int minutes, int seconds) {
	setDate(year, month, day);
	setTime(hour,  minutes, seconds);
}

/**
 * Set time
 */
void Date::setTime(int hour, int minutes, int seconds) {
	setHour(hour);
	setMinutes(minutes);
	setSeconds(seconds);
}

/**
 * Set date
 */
void Date::setDate(int year, int month, int day) {
	setDay(day);
	setMonth(month);
	setYear(year);
}

/**
 * Get/Set parts of date and time
 */
int Date::getSeconds() { return _seconds; }
int Date::getMinutes() { return _minutes; }
int Date::getHour() { return _hour; }
int Date::getDay() { return _day; }
int Date::getMonth() { return _month; }
int Date::getYear() { return _year; }
void Date::setSeconds(int seconds) { _seconds = seconds; }
void Date::setMinutes(int minutes) { _minutes = minutes; }
void Date::setHour(int hour) { _hour = hour; }
void Date::setDay(int day) { _day = day; }
void Date::setMonth(int month) { _month = month; }
void Date::setYear(int year) { _year = year; }

/**
 * Get formatted time
 */
String Date::getTimeString() {
	char hhmmss[] = "00:00:00";
	hhmmss[0] = getHour() / 10 + 48;
	hhmmss[1] = getHour() % 10 + 48;
	hhmmss[3] = getMinutes() / 10 + 48;
	hhmmss[4] = getMinutes() % 10 + 48;
	hhmmss[6] = getSeconds() / 10 + 48;
	hhmmss[7] = getSeconds() % 10 + 48;
	return String(hhmmss);
}

/**
 * Get formatted date
 */
String Date::getDateString() {
	char hhmmss[] = "yy-mm-dd";
	hhmmss[6] = getDay() / 10 + 48;
	hhmmss[7] = getDay() % 10 + 48;
	hhmmss[3] = getMonth() / 10 + 48;
	hhmmss[4] = getMonth() % 10 + 48;
	hhmmss[0] = getYear() / 10 + 48;
	hhmmss[1] = getYear() % 10 + 48;
	return String(hhmmss);
}

/**
 * Get unix timestamp
 */
unsigned long Date::getTimestamp() {
	return DS1337::getTimestamp(getYear(), getMonth(), getDay(), getHour(), getMinutes(), getSeconds());
}

/**
 * Constructor of class DS1337
 */
DS1337::DS1337() {
	//init();
}

/**
 * Init
 */
void DS1337::init() {
	Wire.begin();
	clear();
	_date = Date();
	_alarm = Date();
	_tickMode = DS1337_TICK_UNKNOWN;
	_alarmMode = DS1337_ALARM_UNKNOWN;
}

/**
 * Clear registers
 */
void DS1337::clear() {
	for (int i=0; i<DS1337_REGISTERS; i++) {
		_register[i] = 0;
	}
}

/**
 * Read registers from DS1337
 */
void DS1337::read(int startRegister, int countRegister) {
	Wire.beginTransmission(DS1337_ID);
	Wire.write(startRegister);
	Wire.endTransmission();
	Wire.requestFrom(DS1337_ID, countRegister);
    int reg = startRegister;
	while (Wire.available() > 0) {
		byte b = (byte)Wire.read();
		_register[reg] = b;
		reg++;
	}
}

/**
 * Read date/time registers from DS1337
 */
void DS1337::readDate() {
	read(DS1337_SECONDS, DS1337_REGISTERS_DATE);
}

/**
 * Read alarm 1 registers from DS1337
 */
void DS1337::readAlarm1() {
	read(DS1337_A1_SECONDS, DS1337_REGISTERS_A1);
}

/**
 * Read alarm 2 registers from DS1337
 */
void DS1337::readAlarm2() {
	read(DS1337_A2_MINUTES, DS1337_REGISTERS_A2);
}

/**
 * Read status and control registers from DS1337
 */
void DS1337::readStatus() {
	read(DS1337_CONTROL, DS1337_REGISTERS_STATUS);
}

/**
 * Write date/time registers from DS1337
 */
void DS1337::writeDate() {
	write(DS1337_SECONDS, DS1337_REGISTERS_DATE);
}

/**
 * Write alarm 1 registers from DS1337
 */
void DS1337::writeAlarm1() {
	write(DS1337_A1_SECONDS, DS1337_REGISTERS_A1);
}

/**
 * Write alarm 2 registers from DS1337
 */
void DS1337::writeAlarm2() {
	write(DS1337_A2_MINUTES, DS1337_REGISTERS_A2);
}

/**
 * Write status and control registers from DS1337
 */
void DS1337::writeStatus() {
	write(DS1337_CONTROL, DS1337_REGISTERS_STATUS);
}

/**
 * Write registers to DS1337
 */
void DS1337::write(int startRegister, int countRegister) {
	Wire.beginTransmission(DS1337_ID);
	Wire.write(startRegister);
	for (int i=startRegister; i<(countRegister+startRegister); i++) {
		Wire.write(_register[i]);
	}
	Wire.endTransmission();
}

/**
 * Get day of week (1..7)
 */
int DS1337::getDayOfWeek() {
	readDate();
    return _register[DS1337_DAY_OF_WEEK];
}

/**
 * Set day of week (1..7)
 */
void DS1337::setDayOfWeek(int day) {
	readDate();
    _register[DS1337_DAY_OF_WEEK] = day;
    writeDate();
}

/**
 * Set time (hh:mm)
 */
void DS1337::setTime(int hour, int minutes) {
	setTime(hour, minutes, 0);
}

/**
 * Set time (hh:mm:ss)
 */
void DS1337::setTime(int hour, int minutes, int seconds) {
	Date d = getDate();
	d.setHour(hour);
	d.setMinutes(minutes);
	d.setSeconds(seconds);
	setDate(d);
}

/**
* Set the time with string in form of hh:mm or hh:mm:ss
*/
void DS1337::setTime(String time) {
	time.trim();
	if (time.length()==5)
	setTime(time.substring(0, 2).toInt(), time.substring(3, 5).toInt());
	else if (time.length()==8)
		setTime(time.substring(0, 2).toInt(), time.substring(3, 5).toInt(), time.substring(6, 8).toInt());
}


/**
 * Set date (yy,mm,dd)
 */
void DS1337::setDate(int year, int month, int day) {
	Date d = getDate();
	d.setDay(day);
	d.setMonth(month);
	d.setYear(year);
	setDate(d);
}

/**
* Set the date with string of form yy-mm-dd
*/
void DS1337::setDate(String date) {
	date.trim();
	if (date.length()==8)
	setDate(date.substring(0, 2).toInt(), date.substring(3, 5).toInt(), date.substring(6, 8).toInt());
}

/**
* Set the date and time with string of yy-mm-dd hh:mm
*/
void DS1337::setDateTime(String date) {
	date.trim();
	if (date.length()==14)
	setDateTime(date.substring(0, 2).toInt(), date.substring(3, 5).toInt(), date.substring(6, 8).toInt(), date.substring(9, 11).toInt(), date.substring(12, 14).toInt(), 0);
}

/**
 * Start clock
 */
void DS1337::start() {
	readStatus();
	bitClear(_register[DS1337_CONTROL], DS1337_EOSC);
	writeStatus();
}

/**
 * Stop clock
 */
void DS1337::stop() {
	readStatus();
	bitSet(_register[DS1337_CONTROL], DS1337_EOSC);
	writeStatus();
}

/**
 * Check if clock is running
 */
boolean DS1337::isRunning() {
	readStatus();
	return !bitRead(_register[DS1337_CONTROL], DS1337_EOSC);
}

/**
 * Get the current date
 */
Date DS1337::getDate() {
	readDate();
	_date.setSeconds((_register[DS1337_SECONDS] & 0x0F) + 10 * (_register[DS1337_SECONDS] >> 4));
	_date.setMinutes((_register[DS1337_MINUTES] & 0x0F) + 10 * (_register[DS1337_MINUTES] >> 4));
	_date.setHour((_register[DS1337_HOUR] & 0x0F) + 10 * (_register[DS1337_HOUR] >> 4));
	_date.setDay((_register[DS1337_DAY] & 0x0F) + 10 * (_register[DS1337_DAY] >> 4));
	_date.setMonth((_register[DS1337_MONTH] & 0x0F) + 10 * ((_register[DS1337_MONTH] >> 4) & 0x01));
	_date.setYear((_register[DS1337_YEAR] & 0x0F) + 10 * (_register[DS1337_YEAR] >> 4));
	return _date;
}

/**
 * Set the current date
 */
void DS1337::setDate(Date date) {
	_date = date;
	_register[DS1337_SECONDS] = (date.getSeconds() % 10) + ((date.getSeconds() / 10) << 4);
	_register[DS1337_MINUTES] = (date.getMinutes() % 10) + ((date.getMinutes() / 10) << 4);
	_register[DS1337_HOUR] = (date.getHour() % 10) + ((date.getHour() / 10) << 4);
	_register[DS1337_DAY] = (date.getDay() % 10) + ((date.getDay() / 10) << 4);
	_register[DS1337_MONTH] = (date.getMonth() % 10) + ((date.getMonth() / 10) << 4);
	_register[DS1337_YEAR] = (date.getYear() % 10) + ((date.getYear() / 10) << 4);
	writeDate();
}

/**
 * Set the current date and time
 */
void DS1337::setDateTime(int year, int month, int day, int hour, int minutes, int seconds) {
	readDate();
	_register[DS1337_SECONDS] = (seconds % 10) + ((seconds / 10) << 4);;
	_register[DS1337_MINUTES] = (minutes % 10) + ((minutes / 10) << 4);
	_register[DS1337_HOUR] = (hour % 10) + ((hour / 10) << 4);
	_register[DS1337_DAY] = (day % 10) + ((day / 10) << 4);
	_register[DS1337_MONTH] = (month % 10) + ((month / 10) << 4);
	_register[DS1337_YEAR] = (year % 10) + ((year / 10) << 4);
	writeDate();
}

/**
 * Set the current date and time with a timestamp
 */
void DS1337::setDateTime(unsigned long timestamp) {
    int y,m,d,h,mm,s;
	DS1337::getTime(timestamp, y, m, d, h, mm, s);
    setDateTime(y, m, d, h, mm, s);
}

/**
 * Get the current alarm
 */
Date DS1337::getAlarm() {
	readAlarm1();
	_alarm.setSeconds((_register[DS1337_A1_SECONDS] & 0x0F) + 10 * ((_register[DS1337_A1_SECONDS] >> 4) & 0x07));
	_alarm.setMinutes((_register[DS1337_A1_MINUTES] & 0x0F) + 10 * ((_register[DS1337_A1_MINUTES] >> 4) & 0x07));
	_alarm.setHour((_register[DS1337_A1_HOUR] & 0x0F) + 10 * ((_register[DS1337_A1_HOUR] >> 4) & 0x03));
	_alarm.setDay((_register[DS1337_A1_DAY] & 0x0F) + 10 * ((_register[DS1337_A1_DAY] >> 4) & 0x03));
	return _alarm;
}

/**
 * Set the current alarm
 */
void DS1337::setAlarm(Date date) {
	readAlarm1();
	_alarm = date;
	_register[DS1337_A1_SECONDS] = (_register[DS1337_A1_SECONDS] & 0x80) + (date.getSeconds() % 10) + ((date.getSeconds() / 10) << 4);
	_register[DS1337_A1_MINUTES] = (_register[DS1337_A1_MINUTES] & 0x80) + (date.getMinutes() % 10) + ((date.getMinutes() / 10) << 4);
	_register[DS1337_A1_HOUR] = (_register[DS1337_A1_HOUR] & 0x80) + (date.getHour() % 10) + ((date.getHour() / 10) << 4);
	_register[DS1337_A1_DAY] = (_register[DS1337_A1_DAY] & 0xC0) + (date.getDay() % 10) + ((date.getDay() / 10) << 4);
	writeAlarm1();
}

/**
* Set the alarm time with string in form of hh:mm or dd.hh:mm
*/
void DS1337::setAlarm(String time) {
  time.trim();
  if (time.length()==5)
	  setAlarm(time.substring(0, 2).toInt(), time.substring(3, 5).toInt(), 0);
  else if (time.length()==8)
	  setAlarm(time.substring(0, 2).toInt(), time.substring(3, 5).toInt(), time.substring(6, 8).toInt(), 0);
}

/**
 * Saves the current alarm
 */
void DS1337::saveAlarm() {
	_savedAlarm = Date();
	Date _currentAlarm = getAlarm();
	_savedAlarm.setDay(_currentAlarm.getDay());
	_savedAlarm.setHour(_currentAlarm.getHour());
	_savedAlarm.setMinutes(_currentAlarm.getMinutes());
	_savedAlarm.setSeconds(_currentAlarm.getSeconds());
}

/**
 * Restore the current to the saved alarm
 */
void DS1337::restoreAlarm() {
	setAlarm(_savedAlarm);
}

/**
 * Set the current alarm
 */
void DS1337::setAlarm(int hour, int minutes, int seconds) {
	Date d = getAlarm();
	d.setHour(hour);
	d.setMinutes(minutes);
	d.setSeconds(seconds);
	setAlarm(d);
}

/**
 * Set the current alarm
 */
void DS1337::setAlarm(int day, int hour, int minutes, int seconds) {
	Date d = getAlarm();
	d.setDay(day);
	d.setHour(hour);
	d.setMinutes(minutes);
	d.setSeconds(seconds);
	setAlarm(d);
}

/**
 * Snooze some minutes (between 1 .. 60)
 * Alarm is enabled at current alarm + minutes
 */
void DS1337::snooze(int minutes) {
	clearAlarm();
	if (minutes>0 && minutes<=60) {
		Date _currentAlarm = getAlarm();
		_currentAlarm.setMinutes(_currentAlarm.getMinutes()+minutes);
		if (_currentAlarm.getMinutes()>60) {
			_currentAlarm.setMinutes(_currentAlarm.getMinutes()-60);
			_currentAlarm.setHour(_currentAlarm.getHour()+1);
			if (_currentAlarm.getHour()>23) {
				_currentAlarm.setHour(0);
				_currentAlarm.setDay(_currentAlarm.getDay()+1);
			}
		}
		setAlarm(_currentAlarm);
	}
}

/**
 * Enable alarm
 */
void DS1337::enableAlarm() {
	readStatus();
	bitSet(_register[DS1337_CONTROL], DS1337_A1IE);
	writeStatus();
}

/**
 * Disable alarm
 */
void DS1337::disableAlarm() {
	readStatus();
	bitClear(_register[DS1337_CONTROL], DS1337_A1IE);
	writeStatus();
}

/**
 * Clear alarm
 */
void DS1337::clearAlarm() {
	readStatus();
	bitClear(_register[DS1337_STATUS], DS1337_A1F);
	writeStatus();
}

/**
 * Toggle alarm
 */
 void DS1337::toggleAlarm() {
	 if (isAlarmEnabled())
		 disableAlarm();
	 else
		 enableAlarm();
 }

/**
 * Check, if alarm is enabled
 */
boolean DS1337::isAlarmEnabled() {
	readStatus();
	return bitRead(_register[DS1337_CONTROL], DS1337_A1IE);
}

/**
 * Check, if alarm is active
 */
boolean DS1337::isAlarmActive() {
	readStatus();
	return bitRead(_register[DS1337_STATUS], DS1337_A1F);
}

/**
 * Check, if tick is active
 */
boolean DS1337::isTickActive() {
	readStatus();
	return bitRead(_register[DS1337_STATUS], DS1337_A2F);
}


/**
 * Set the tick mode (every second or minuzte)
 */
void DS1337::setTickMode(int tickMode) {
	if (tickMode==DS1337_NO_TICKS) {
		readStatus();
		bitSet(_register[DS1337_CONTROL], DS1337_INTCN);
		bitClear(_register[DS1337_CONTROL], DS1337_A2IE);
		bitClear(_register[DS1337_STATUS], DS1337_A2F);
		writeStatus();
		_tickMode = tickMode;
	}
	else if (tickMode==DS1337_TICK_EVERY_SECOND) {
		readStatus();
		bitClear(_register[DS1337_CONTROL], DS1337_INTCN);
		bitClear(_register[DS1337_CONTROL], DS1337_A2IE);
		bitClear(_register[DS1337_CONTROL], DS1337_RS1);
		bitClear(_register[DS1337_CONTROL], DS1337_RS2);
		bitClear(_register[DS1337_STATUS], DS1337_A2F);
		writeStatus();
		_tickMode = tickMode;
	}
	else if (tickMode==DS1337_TICK_EVERY_MINUTE) {
		readStatus();
		readAlarm2();
		bitSet(_register[DS1337_CONTROL], DS1337_INTCN);
		bitSet(_register[DS1337_CONTROL], DS1337_A2IE);
		bitClear(_register[DS1337_STATUS], DS1337_A2F);
		bitSet(_register[DS1337_A2_MINUTES], DS1337_A2M2);
		bitSet(_register[DS1337_A2_HOUR], DS1337_A2M3);
		bitSet(_register[DS1337_A2_DAY], DS1337_A2M4);
		writeAlarm2();
		writeStatus();
		_tickMode = tickMode;
	}
	else if (tickMode==DS1337_TICK_EVERY_HOUR) {
		readStatus();
		readAlarm2();
		bitSet(_register[DS1337_CONTROL], DS1337_INTCN);
		bitSet(_register[DS1337_CONTROL], DS1337_A2IE);
		bitClear(_register[DS1337_STATUS], DS1337_A2F);
		//bitClear(_register[DS1337_A2_MINUTES], DS1337_A2M2);
		_register[DS1337_A2_MINUTES] = 0;
		bitSet(_register[DS1337_A2_HOUR], DS1337_A2M3);
		bitSet(_register[DS1337_A2_DAY], DS1337_A2M4);
		writeAlarm2();
		writeStatus();
		_tickMode = tickMode;
	}
}

/**
 * Get the tick mode (second | minute)
 */
int DS1337::getTickMode() {
	readStatus();
	readAlarm2();
	bool intcn = bitRead(_register[DS1337_CONTROL], DS1337_INTCN);
	bool a1ie = bitRead(_register[DS1337_CONTROL], DS1337_A1IE);
	bool a2ie = bitRead(_register[DS1337_CONTROL], DS1337_A2IE);
	bool rs1 = bitRead(_register[DS1337_CONTROL], DS1337_RS1);
	bool rs2 = bitRead(_register[DS1337_CONTROL], DS1337_RS2);
	bool a2m2 = bitRead(_register[DS1337_A2_MINUTES], DS1337_A2M2);
	bool a2m3 = bitRead(_register[DS1337_A2_HOUR], DS1337_A2M3);
	bool a2m4 = bitRead(_register[DS1337_A2_DAY], DS1337_A2M4);
	bool dydt = bitRead(_register[DS1337_A2_DAY], DS1337_A2DYDT);
	if (!intcn && !rs1 && !rs2 && !a2ie)
		_tickMode = DS1337_TICK_EVERY_SECOND;
	else if (intcn && !a2ie)
		_tickMode = DS1337_NO_TICKS;
	else if (intcn && a2ie && a2m4 && a2m3 && a2m2)
		_tickMode = DS1337_TICK_EVERY_MINUTE;
	else if (intcn && a2ie && a2m4 && a2m3 && _register[DS1337_A2_MINUTES]==0)
		_tickMode = DS1337_TICK_EVERY_HOUR;
	else
		_tickMode = DS1337_TICK_UNKNOWN;
	return _tickMode;
}

/**
 * Reset the tick flag (must be done in hour and minite tick mode)
 */
void DS1337::resetTick() {
	readStatus();
	bitClear(_register[DS1337_STATUS], DS1337_A2F);
	writeStatus();
}

/**
 * Get the value of one of the sixteen DS1337 register
 */
int DS1337::getRegister(int i) {
	read(i, 1);
	return _register[i];
}

/**
 * Check, if clock has stopped (Oscillator Stop Flag is set)
 */
boolean DS1337::hasStopped() {
	readStatus();
	return bitRead(_register[DS1337_STATUS], DS1337_OSF);
}

/**
 * Clear the OSF (Oscillator Stop Flag)
 */
void DS1337::clearOSF() {
	readStatus();
	bitClear(_register[DS1337_STATUS], DS1337_OSF);
	writeStatus();
}

/**
 * Clear all flags
 */
void DS1337::clearFlags() {
	readStatus();
	bitClear(_register[DS1337_STATUS], DS1337_OSF);
	bitClear(_register[DS1337_STATUS], DS1337_A2F);
	bitClear(_register[DS1337_STATUS], DS1337_A1F);
	writeStatus();
}

/**
 * Set the alarm mode
 */
void DS1337::setAlarmMode(int alarmMode) {
	readAlarm1();
	switch(alarmMode) {
		case DS1337_ALARM_EVERY_SECOND:
			bitSet(_register[DS1337_A1_SECONDS], DS1337_A1M1);
			bitSet(_register[DS1337_A1_MINUTES], DS1337_A1M2);
			bitSet(_register[DS1337_A1_HOUR], DS1337_A1M3);
			bitSet(_register[DS1337_A1_DAY], DS1337_A1M4);
			bitClear(_register[DS1337_A1_DAY], DS1337_A1DYDT);
			break;
		case DS1337_ALARM_ON_SECOND:
			bitClear(_register[DS1337_A1_SECONDS], DS1337_A1M1);
			bitSet(_register[DS1337_A1_MINUTES], DS1337_A1M2);
			bitSet(_register[DS1337_A1_HOUR], DS1337_A1M3);
			bitSet(_register[DS1337_A1_DAY], DS1337_A1M4);
			bitClear(_register[DS1337_A1_DAY], DS1337_A1DYDT);
			break;
		case DS1337_ALARM_ON_SECOND_MINUTE:
			bitClear(_register[DS1337_A1_SECONDS], DS1337_A1M1);
			bitClear(_register[DS1337_A1_MINUTES], DS1337_A1M2);
			bitSet(_register[DS1337_A1_HOUR], DS1337_A1M3);
			bitSet(_register[DS1337_A1_DAY], DS1337_A1M4);
			bitClear(_register[DS1337_A1_DAY], DS1337_A1DYDT);
			break;
		case DS1337_ALARM_ON_SECOND_MINUTE_HOUR:
			bitClear(_register[DS1337_A1_SECONDS], DS1337_A1M1);
			bitClear(_register[DS1337_A1_MINUTES], DS1337_A1M2);
			bitClear(_register[DS1337_A1_HOUR], DS1337_A1M3);
			bitSet(_register[DS1337_A1_DAY], DS1337_A1M4);
			bitClear(_register[DS1337_A1_DAY], DS1337_A1DYDT);
			break;
		case DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DATE:
			bitClear(_register[DS1337_A1_SECONDS], DS1337_A1M1);
			bitClear(_register[DS1337_A1_MINUTES], DS1337_A1M2);
			bitClear(_register[DS1337_A1_HOUR], DS1337_A1M3);
			bitClear(_register[DS1337_A1_DAY], DS1337_A1M4);
			bitClear(_register[DS1337_A1_DAY], DS1337_A1DYDT);
			break;
		case DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DAY:
			bitClear(_register[DS1337_A1_SECONDS], DS1337_A1M1);
			bitClear(_register[DS1337_A1_MINUTES], DS1337_A1M2);
			bitClear(_register[DS1337_A1_HOUR], DS1337_A1M3);
			bitClear(_register[DS1337_A1_DAY], DS1337_A1M4);
			bitSet(_register[DS1337_A1_DAY], DS1337_A1DYDT);
			break;
	}
	_alarmMode = alarmMode;
	writeAlarm1();
}

/**
 * Get the alarm mode
 */
int DS1337::getAlarmMode() {
	readAlarm1();
	bool a1m1 = bitRead(_register[DS1337_A1_SECONDS], DS1337_A1M1);
	bool a1m2 = bitRead(_register[DS1337_A1_MINUTES], DS1337_A1M2);
	bool a1m3 = bitRead(_register[DS1337_A1_HOUR], DS1337_A1M3);
	bool a1m4 = bitRead(_register[DS1337_A1_DAY], DS1337_A1M4);
	bool dydt = bitRead(_register[DS1337_A1_DAY], DS1337_A1DYDT);
	if (a1m1 && a1m2 && a1m3 && a1m4)
		_alarmMode = DS1337_ALARM_EVERY_SECOND;
	else if (!a1m1 && a1m2 && a1m3 && a1m4)
		_alarmMode = DS1337_ALARM_ON_SECOND;
	else if (!a1m1 && !a1m2 && a1m3 && a1m4)
		_alarmMode = DS1337_ALARM_ON_SECOND_MINUTE;
	else if (!a1m1 && !a1m2 && !a1m3 && a1m4)
		_alarmMode = DS1337_ALARM_ON_SECOND_MINUTE_HOUR;
	else if (!a1m1 && !a1m2 && !a1m3 && !a1m4 && !dydt)
		_alarmMode = DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DATE;
	else if (!a1m1 && !a1m2 && !a1m3 && !a1m4 && dydt)
		_alarmMode = DS1337_ALARM_ON_SECOND_MINUTE_HOUR_DAY;
	else
		_alarmMode = DS1337_ALARM_UNKNOWN;
	return _alarmMode;
}

/**
 * Convert from unix timestamp
 */
void DS1337::getTime(unsigned long timestamp, int &year, int &month, int &day, int &hour, int &minute, int &second) {
	timestamp-= T2000UTC;
	second = (int)(timestamp % 60);
	timestamp = timestamp / 60;
	minute = (int)(timestamp % 60);
	timestamp = timestamp / 60;
	hour = (int)(timestamp % 24);
	timestamp = timestamp / 24;
	year = 0;
	int dYear = 0;
	if (year % 4 == 0)
		dYear = 366;
	else
		dYear = 365;
	while (timestamp >= dYear) {
		year++;
		timestamp-=dYear;
		if (year % 4 == 0)
			dYear = 366;
		else
			dYear = 365;
	}
	day = (int) timestamp;
	int dayMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	month = 0;
	while (day > dayMonth[month+1])
		month++;
	day-=dayMonth[month];
	month++;
	day++;
}

/**
 * Get unix timestamp
 */
unsigned long DS1337::getTimestamp(int year, int month, int day, int hour, int minute, int second) {
  unsigned long secMinute = 60;
  unsigned long secHour = 60 * secMinute;
  unsigned long secDay = 24 * secHour;
  unsigned long secYear = 365 * secDay;
  int dayMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

  // leap years
  int leapDays = (year >> 2) + 1;
  if (((year & 3) == 0) && month < 3)
    leapDays--;

  // calculate
  return year * secYear + (dayMonth[month-1] + (day-1) + leapDays) * secDay + hour * secHour + minute * secMinute + second + T2000UTC;
}

/**
 * Get unix timestamp
 */
unsigned long DS1337::getTimestamp() {
	Date d = getDate();
	return getTimestamp(d.getYear(), d.getMonth(), d.getDay(), d.getHour(), d.getMinutes(), d.getSeconds());
}

