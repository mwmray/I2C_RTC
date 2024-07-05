/*
 * DS1307.h
 *
 *  Created on: Jun 30, 2024
 *      Author: M.W Laptop
 */

#ifndef DEVICE_DRIVERS_DS1307_DS1307_H_
#define DEVICE_DRIVERS_DS1307_DS1307_H_
#include <stdint.h>
#include "stm32f1xx_hal.h"

typedef struct {
	uint8_t I2C_Buffer[8];
	uint8_t sec, min, hour, day, date, month, year;
	uint8_t CH; //clock hold
	uint8_t format:1;
	uint8_t PM_AM:1;
	I2C_HandleTypeDef *I2C_BUS;
} ds1307_t;

typedef enum {
	DS1307_SUCCESS, DS1307_FAILED,
} ds1307_stat_t;

ds1307_stat_t Ds1307_Init(ds1307_t *clock, I2C_HandleTypeDef *I2C_BUS);
ds1307_stat_t Ds1307_Read_Time(ds1307_t *clock);
ds1307_stat_t Ds1307_Write_Time(ds1307_t *clock);
#endif /* DEVICE_DRIVERS_DS1307_DS1307_H_ */
