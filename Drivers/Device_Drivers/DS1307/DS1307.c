/*
 * DS1307.C
 *
 *  Created on: Jun 30, 2024
 *      Author: M.W Laptop
 */
#include "DS1307.h"
#define RTC_SLA 0b1101000
#define start_address 0x00

static uint8_t BCD2DEC(uint8_t num) {
	return (num >> 4) * 10 + (0xf & num);
}

static uint8_t DEC2BCD(uint8_t num) {
	uint8_t div = num / 10;
	uint8_t rem = num % 10;
	return div << 4 | rem;
}

ds1307_stat_t Ds1307_Init(ds1307_t *clock) {
	uint8_t stat = 1;
	//enable oscillator
	clock->I2C_Buffer[0] = 0X00;
	stat &= clock->AG_HW_Interface.Write_time(RTC_SLA,clock->I2C_Buffer, 1);
	stat &= clock->AG_HW_Interface.Read_time(RTC_SLA,clock->I2C_Buffer, 1);

	if ((clock->I2C_Buffer[0]) & (1 << 7) == 0) {

	} else {
		clock->I2C_Buffer[0] = 0x00; //reg base address
		clock->I2C_Buffer[1] = 0; // clock hold bit
		stat &= clock->AG_HW_Interface.Write_time(RTC_SLA,clock->I2C_Buffer, 2);
	}

	if (stat == 1) {
		return DS1307_SUCCESS;
	} else {
		return DS1307_FAILED;
	}
}
ds1307_stat_t Ds1307_Read_Time(ds1307_t *clock) {
	//uint8_t stat = 1;
	clock->I2C_Buffer[0] = start_address;
	if (clock->AG_HW_Interface.Write_time(RTC_SLA,clock->I2C_Buffer, 1) == 1) {
		if (clock->AG_HW_Interface.Read_time(RTC_SLA,clock->I2C_Buffer, 7) == 1) {
			clock->sec = BCD2DEC(clock->I2C_Buffer[0] & (0x7f));
			clock->min = BCD2DEC(clock->I2C_Buffer[1]);
			clock->format = clock->I2C_Buffer[2] & 0b01000000 >> 6;
			if (clock->format == 1) { //12 h form
				clock->hour = BCD2DEC(clock->I2C_Buffer[2] & 0b00011111);
				clock->PM_AM = (clock->I2C_Buffer[2] & 0b00100000) >> 5;
			} else {
				clock->PM_AM = (clock->I2C_Buffer[2] & 0b00111111) >> 5;
				if (clock->hour > 11) {
					clock->PM_AM = 1;
				} else {
					clock->PM_AM = 0;
				}
			}
			clock->day = BCD2DEC(clock->I2C_Buffer[3] & 0x7);
			clock->date = BCD2DEC(clock->I2C_Buffer[4] & 0x3f);
			clock->month = BCD2DEC(clock->I2C_Buffer[5] & 0x1f);
			clock->year = BCD2DEC(clock->I2C_Buffer[6]) + 2000;
			return DS1307_SUCCESS;
		} else {
			return DS1307_FAILED;
		}
	} else {
		return DS1307_FAILED;
	}
}
ds1307_stat_t Ds1307_Write_Time(ds1307_t *clock) {
	clock->I2C_Buffer[0] = start_address;
	clock->I2C_Buffer[1] = DEC2BCD(clock->sec);
	clock->I2C_Buffer[2] = DEC2BCD(clock->min);
	if (clock->format == 1) {
		clock->I2C_Buffer[3] = DEC2BCD(clock->hour) | (clock->format << 6)
				| (clock->PM_AM << 5);
	} else {
		clock->I2C_Buffer[3] = DEC2BCD(clock->hour) | (clock->format << 6);
	}
	clock->I2C_Buffer[4] = DEC2BCD(clock->day & 0x7);
	clock->I2C_Buffer[5] = DEC2BCD(clock->date & 0x3f);
	clock->I2C_Buffer[6] = DEC2BCD(clock->month & 0x1f);
	clock->I2C_Buffer[7] = DEC2BCD(clock->year) - 2000;

	if (clock->AG_HW_Interface.Write_time(RTC_SLA,clock->I2C_Buffer, 1) == 1) {
		return DS1307_SUCCESS;
	} else {
		return DS1307_FAILED;
	}

}
