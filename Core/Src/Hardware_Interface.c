/*
 * Hardware_Interface.c
 *
 *  Created on: Sep 1, 2024
 *      Author: M.W Laptop
 */

#include "Hardware_Interface.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t Read_time(uint8_t sla, uint8_t *Data, uint8_t Length) {
	HAL_StatusTypeDef ok = HAL_I2C_Master_Receive(&hi2c1, sla << 1, Data,
			Length, 100);
	return (ok == HAL_OK) ? 1 : 0;
}
uint8_t Write_time(uint8_t sla, uint8_t *Data, uint8_t Length) {
	HAL_StatusTypeDef ok = HAL_I2C_Master_Transmit(&hi2c1, sla << 1, Data,
			Length, 100);
	return (ok == HAL_OK) ? 1 : 0;
}

