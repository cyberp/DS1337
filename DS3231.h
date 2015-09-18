/**

DS3231.h

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

#ifndef DS3231_h
#define DS3231_h

// includes
#include "DS1337.h"
#include <Arduino.h>

// additional DS3231 registers
#define DS3231_REGISTERS   		19
#define DS3231_REGISTERS_TEMP    2
#define DS3231_AGING_OFFSET   0x10
#define DS3231_TEMP_MSB       0x11
#define DS3231_TEMP_LSB       0x12

// additional DS3231 status register flags
#define DS3231_EN32KHZ		0x03
#define DS3231_BSY			0x02

// additional DS3231 control register flags
#define DS3231_CONV			0x05

// class definition of DS3231 RTC
class DS3231 : public DS1337 {
public:
	void enableAlarm();
	void disableAlarm();
	boolean isAlarmEnabled();
	void enable32kHz();
	void disable32kHz();
	bool is32kHzEnabled();
	bool toggle32kHz();
	void clear();
	float getTemperature();
	void startConversion();
private:
};

#endif
