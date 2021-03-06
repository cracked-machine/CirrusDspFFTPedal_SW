/*
 * user_input.c
 *
 *  Created on: Nov 7, 2020
 *      Author: chris
 */

#include "user_input.h"

uint8_t active_adc_channel = 0;
uint8_t test = 0;


void check_user_buttons_state()
{
	// process user switch input values
	if(user_switch1_GPIO_Port->IDR & user_switch1_Pin)
	  LEDA_BLUE_GPIO_Port->ODR |= LEDA_BLUE_MASK;
	else
	  LEDA_BLUE_GPIO_Port->ODR &= ~LEDA_BLUE_MASK;


	if(user_switch2_GPIO_Port->IDR & user_switch2_Pin)
	  LEDB_BLUE_GPIO_Port->ODR |= LEDB_BLUE_MASK;
	else
	  LEDB_BLUE_GPIO_Port->ODR &= ~LEDB_BLUE_MASK;


//	printf("%i\r\n", test);
//	test++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// check origin of EXTI
	switch(GPIO_Pin){

		case GPIO_PIN_13:
			// enable relay, set green led, unset red led
			relay_switch_GPIO_Port->ODR |= RELAY_MASK;
			LEDA_GREEN_GPIO_Port->ODR |= LEDA_GREEN_MASK;
			LEDA_RED_GPIO_Port->ODR &= ~LEDA_RED_MASK;
			LEDB_GREEN_GPIO_Port->ODR |= LEDB_GREEN_MASK;
			LEDB_RED_GPIO_Port->ODR &= ~LEDB_RED_MASK;
			break;

		case GPIO_PIN_14:
			// disable relay, set red led, unset green led
			relay_switch_GPIO_Port->ODR &= ~RELAY_MASK;
			LEDA_GREEN_GPIO_Port->ODR &= ~LEDA_GREEN_MASK;
			LEDA_RED_GPIO_Port->ODR |= LEDA_RED_MASK;
			LEDB_GREEN_GPIO_Port->ODR &= ~LEDB_GREEN_MASK;
			LEDB_RED_GPIO_Port->ODR |= LEDB_RED_MASK;
			break;
	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	//printf("%lu\r\n", adc_input_data_buffer[ADC_DATA_BUFFER_SIZE-1]);
	//	adc_input_data_demuxed[active_adc_channel] = adc_input_data_buffer[0];

	// demux interleaved ADC input
	switch(active_adc_channel)
	{
		case 0:
			user_input_data.exp_input = (uint32_t)adc_input_data_buffer[0];
			break;

		case 1:
			user_input_data.user_pot1 = (uint32_t)adc_input_data_buffer[0];
			break;

		case 2:
			user_input_data.user_pot2 = (uint32_t)adc_input_data_buffer[0];
			break;
	}

	active_adc_channel++;
	if(active_adc_channel > ADC_DATA_DEMUX_SIZE-1)
		active_adc_channel = 0;

}

