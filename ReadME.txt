DS3231RTC driver is made with a easy to understand interface.
/*
	Name			: DS3231RTC_driver.ino
	Created			: 14-Jan-19 20:46:08
	Author			: Oscar goos
	Code			: C++, Flash: 3944B, RAM: 413B
	Contrcoller		: ATmega328
	Last tested		: 14-Jan-2019
	Notes			: Only Read Write Time and date registers implemented
				: DS3231reg_t Represents all DS3231 registers for date an time and is a memeory copy of the chip
				: RTC_t is the software respresentation of the RTC CHIP read for the last time.
				: The  DS3231RTC_driver(SYNC) copies the Time and date from the RTC chip to the RTC_t
				: The  DS3231RTC_driver(INIT) copies the Time and date from the RTC_t to the RTC chip
				: RTC_t is public and be used together with INIT to set the time of the chip.
				: RTC_t.year is entered as four digits year. 
				: DS3231.YinC is stored as two digits=(YinC=Year in Century);
				: All BCD converstion are done on the fly and are invisible for the user.
				: Temperature not implemented
*/
