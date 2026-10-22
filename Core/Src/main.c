/*******************************************************************************
 * @file           : main.c
 * @brief          : Main program body for assignment 4
 *******************************************************************************/


#include "main.h"
#include "stm32l476xx.h"

void spi_init(){

	// configure MOSI, MISO, SCK pins
	GPIOA->MODER &= ~(0x3U << )

}


int main(void)
{
	HAL_Init();
	SystemClock_Config();
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN); // Enable GPIOA clock

	GPIOA->MODER &= ~(0x3U << (TIMER_PIN*2)); // clearing PA5
	GPIOA->MODER|=GPIO_MODER_MODE5_0;//set PA5 as output -- can monitor clock cycles with this pin

	// set P5 HIGH 
	GPIOA->ODR |= (1 << TIMER_PIN);
	

	timer_init();


	while(1) {

	}

 }
