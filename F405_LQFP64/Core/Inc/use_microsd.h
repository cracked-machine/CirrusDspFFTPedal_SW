/*
 * read_microsd_data.h
 *
 *  Created on: Nov 8, 2020
 *      Author: chris
 */

#ifndef INC_USE_MICROSD_H_
#define INC_USE_MICROSD_H_


#include "main.h"
#include "stm32f4xx_hal.h"

void read_sdio_data();
void write_sdio_data(char* filename, char* text);
void error_sdio(char* opcode);

#endif /* INC_USE_MICROSD_H_ */
