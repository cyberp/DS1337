/**

DS3231.cpp

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
#include "DS3231.h"
#include "Wire.h"

/**
 * Enable alarm
 */
void DS3231::enableAlarm() {
	readStatus();
	bitSet(_register[DS1337_CONTROL], DS1337_A1IE);
	bitSet(_register[DS1337_CONTROL], DS1337_INTCN);
	writeStatus();
}

/**
 * Enable alarm
 */
void DS3231::disableAlarm() {
	readStatus();
	bitClear(_register[DS1337_CONTROL], DS1337_A1IE);
	writeStatus();
}

/**
 * Check, if alarm is enabled
 */
boolean DS3231::isAlarmEnabled() {
	readStatus();
	return (bitRead(_register[DS1337_CONTROL], DS1337_A1IE) && bitRead(_register[DS1337_CONTROL], DS1337_INTCN));
}

/**
 * Enable 32KHz signal
 */
void DS3231::enable32kHz() {
	readStatus();
	bitSet(_register[DS1337_STATUS], DS3231_EN32KHZ);
	writeStatus();
}

/**
 * Disable 32KHz signal
 */
void DS3231::disable32kHz() {
	readStatus();
	bitClear(_register[DS1337_STATUS], DS3231_EN32KHZ);
	writeStatus();
}

/**
 * Check, if 32KHz signal is enabled
 */
bool DS3231::is32kHzEnabled() {
	readStatus();
	return bitRead(_register[DS1337_STATUS], DS3231_EN32KHZ);
}

/**
 * Toggle the 32KHz signal
 */
bool DS3231::toggle32kHz() {
	if (is32kHzEnabled())
		disable32kHz();
	else
		enable32kHz();
}

/**
 * Clear registers
 */
void DS3231::clear() {
	for (int i=0; i<DS3231_REGISTERS; i++) {
		_register[i] = 0;
	}
}

/**
 * Get the temperature
 */
float DS3231::getTemperature() {
	read(DS3231_TEMP_MSB, DS3231_REGISTERS_TEMP);
	char c = _register[DS3231_TEMP_MSB];
	float f = (float)(_register[DS3231_TEMP_LSB] >> 6) / 4.0;
	float t = (float)c + f;
	return t;
}

/**
 * Start conversion of temperature
 */
void DS3231::startConversion() {
	readStatus();
	if(!bitRead(_register[DS1337_STATUS], DS3231_BSY)) {
		bitSet(_register[DS1337_CONTROL], DS3231_CONV);
		writeStatus();
	}
}
