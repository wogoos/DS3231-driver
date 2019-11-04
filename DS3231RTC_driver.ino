DS3231RTC driver is made with a easy to understand interface.
/*
	Name		: DS3231RTC_driver.ino
	Created		: 14-Jan-19 20:46:08
	Author		: Oscar goos
	Code		: C++, Flash: 3944B, RAM: 413B
	Contrcoller	: ATmega328
	Last tested	: 14-Jan-2019
	Notes		: Only Read Write Time and date registers implemented
	-			: DS3231reg_t Represents all DS3231 registers for date an time and is a memeory copy of the chip
	-			: RTC_t is the software respresentation of the RTC CHIP read for the last time.
	-			: The  DS3231RTC_driver(SYNC) copies the Time and date from the RTC chip to the RTC_t
	-			: The  DS3231RTC_driver(INIT) copies the Time and date from the RTC_t to the RTC chip
	-			: RTC_t is public and be used together with INIT to set the time of the chip.
	-			: RTC_t.year is entered as four digits year. DS3231.YinC is stored as two digitis (YinC=Year in Century);
	-			: All BCD converstion are done on the fly and are invisible for the user.
	-			: Temperature not implemented
*/

#include <Wire.h>         

#define		INIT					0
#define		SYNC					1
#define		GET						2
//#define		

#define		I2C_EEprom_ID 			0x50
#define		I2C_DS3231_ADR 			0x68
#define		STOP					true

#define		BCD2DEC(bcdval)			(((bcd_val>>4)*10) + (bcd_val & 0x0F))
#define		DEC2BCD(decval)			(((dec_val/10)<<4) + (dec_val%10))


typedef struct DS3231reg{
	uint8_t						sec;	// seconds,			00..59
	uint8_t						min;	// minuts,			00..59	
	uint8_t						hr;		// hours,			00..23
	uint8_t						DinW;	// Day in week,		01..07
	uint8_t						DinM;	// Day in month,	01..31
	uint8_t						MinY;	// Month in year,	01..12		
	uint16_t					YinC;	// Year in century,	00..99 respresenting 2000..2099)	
} DS3231reg_t;

DS3231reg_t						DS3231;

typedef struct RTC_t{
	uint8_t						sec;	// seconds,			00..59
	uint8_t						min;	// minuts,			00..59	
	uint8_t						hr;		// hours,			00..23
	uint8_t						DinW;	// Day in week,		01..07
	uint8_t						DinM;	// Day in month,	01..31
	uint8_t						MinY;	// Month in year,	01..12		
	uint16_t					year;	// Year				2000..2100
	uint32_t					SinD;	// seconds in day,	00..86399
} RTC_t;

RTC_t							RTC = { 13, 20, 10, 6, 11, 12, 2019, (uint32_t)RTC.hr*3600 + (uint32_t)RTC.min*60 +  (uint32_t)RTC.sec};

void setup(void)
{
	Serial.begin(115200);
	Wire.begin();
	Serial.println(RTC.SinD);
	DS3231RTC_driver(INIT);
	delay(7000);
	DS3231RTC_driver(SYNC);
	Serial.println(RTC.sec);
}

void loop(void)
{
    delay(1000);
}


void DS3231RTC_driver(uint8_t cmd) {
	
	uint8_t   n, bcd_val, dec_val;
	uint8_t	 *DSptr = (uint8_t*)&DS3231;
	
	switch (cmd){
		case INIT: {
			DS3231.sec	=	RTC.sec;
			DS3231.min	=	RTC.min;
			DS3231.hr	=	RTC.hr;
			DS3231.DinM	=	RTC.DinM;
			DS3231.MinY	=	RTC.MinY;
			DS3231.YinC	=   RTC.year-2000;
			DS3231.DinW	=	RTC.DinW;
			Wire.beginTransmission(I2C_DS3231_ADR)	;						// Start I2C and send Address
			Wire.write(0);													// set DS3231 register pointer to 0
			for(n=0; n < sizeof(DS3231); n++) {
				dec_val= *DSptr++;											// Read nth data element from DS3231
				Wire.write(DEC2BCD(dec_val));								// Convert nth element dec2bcd and set all 7 RTC  Time date registers
			}
			Wire.endTransmission();											// Send I2C STOP and release the bus
		} break;
		
		case SYNC: {
			Wire.beginTransmission(I2C_DS3231_ADR);							// Start I2C and send Address
			Wire.write(0);													// set RTC  register address pointer to 0
			Wire.endTransmission();
			Wire.requestFrom(I2C_DS3231_ADR, sizeof(DS3231), STOP);
			for(n=0; n < sizeof(DS3231); n++) {
				bcd_val   = Wire.read();
				*DSptr++  = BCD2DEC(bcd_val);									// Read all 7 RTC  registers, convert bcd2dec in
			}
			RTC.sec		=	DS3231.sec;
			RTC.min		=	DS3231.min;
			RTC.hr		=	DS3231.hr;
			RTC.DinM	=	DS3231.DinM;
			RTC.MinY	=	DS3231.MinY;
			RTC.year	=	DS3231.YinC +2000;
			RTC.DinW	=	DS3231.DinW;
		} break;
	} 
}
