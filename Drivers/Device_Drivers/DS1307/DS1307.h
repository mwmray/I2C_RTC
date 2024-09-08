/*
 * DS1307.h
 *
 *  Created on: Jun 30, 2024
 *      Author: M.W Laptop
 */

#ifndef DEVICE_DRIVERS_DS1307_DS1307_H_
#define DEVICE_DRIVERS_DS1307_DS1307_H_
#include <stdint.h>


typedef struct {
	uint8_t I2C_Buffer[8];
	uint8_t sec, min, hour, day, date, month;
	uint16_t year;
	uint8_t CH; //clock hold
	uint8_t format:1;
	uint8_t PM_AM:1;
	struct HW_Interface{
		uint8_t (*Read_time)(uint8_t sla,uint8_t *Data, uint8_t Length);
		uint8_t (*Write_time)(uint8_t sla,uint8_t *Data, uint8_t Length);
	}AG_HW_Interface;
} ds1307_t;

typedef enum {
	DS1307_SUCCESS, DS1307_FAILED,
} ds1307_stat_t;

ds1307_stat_t Ds1307_Init(ds1307_t *clock);
ds1307_stat_t Ds1307_Read_Time(ds1307_t *clock);
ds1307_stat_t Ds1307_Write_Time(ds1307_t *clock);
#endif /* DEVICE_DRIVERS_DS1307_DS1307_H_ */
