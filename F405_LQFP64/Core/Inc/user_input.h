/*
 * user_input.h
 *
 *  Created on: Nov 7, 2020
 *      Author: chris
 */

#ifndef INC_USER_INPUT_H_
#define INC_USER_INPUT_H_

#include "main.h"
#include "stm32f4xx_hal.h"

// Should always be 1 for STM32F4 (Cortex-M4 ADCs interleave all channel inputs into single variable)
#define ADC_DATA_BUFFER_SIZE 1
// The buffer that stores the raw ADC input data
uint32_t adc_input_data_buffer[ADC_DATA_BUFFER_SIZE];

// Can be 1-16 channels
#define ADC_DATA_DEMUX_SIZE 3

// size *must* match ADC_DATA_DEMUX_SIZE!
typedef struct
{
	uint32_t exp_input;
	uint32_t user_pot1;
	uint32_t user_pot2;
} adc_input_data_demuxed_t;

// the de-interleaved data store
adc_input_data_demuxed_t user_input_data;


#define RELAY_MASK GPIO_ODR_OD0_Msk

#define LEDA_RED_MASK GPIO_ODR_OD0_Msk
#define LEDA_BLUE_MASK GPIO_ODR_OD1_Msk
#define LEDA_GREEN_MASK GPIO_ODR_OD2_Msk
#define LEDB_RED_MASK GPIO_ODR_OD10_Msk
#define LEDB_BLUE_MASK GPIO_ODR_OD11_Msk
#define LEDB_GREEN_MASK GPIO_ODR_OD12_Msk

void check_user_buttons_state();


#endif /* INC_USER_INPUT_H_ */
