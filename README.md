# DS1337 / DS3231 RTC Library

Written by Christian Paul, 2014 - 2015.
This software is released under the terms of the MIT license.
See the file LICENSE or LIZENZ for details, please.

This library works the for two RTC IC - the DS1337 and [DS3231](https://oberguru.net/elektronik/rtc/ds3231/ds3231.html). 

Both IC have two configurable alarms. Both only the DS1337 have two pins for each alarm. On the DS3231 both alarm shares one pin.

The DS3231 have an extra 32kHz signal and measures the temperature of the environment.

With this library you can set one of the alarms.
Additional, it's possible to set a tick that occurs every second or every minute. 
But only on the DS1337 it's possible to use the alarm and the tick on every second together. On DS3231 you can use the tick every seconds or the alarm and a tick every minute.

Include always DS1337.h/DS3231.h and Wire.h in your projects. Only DS1337.h or DS3231.h will not work.

See examples for using the software.

