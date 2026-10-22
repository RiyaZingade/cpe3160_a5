/*******************************************************************************
 * @file           : main.c
 * @brief          : Main program body for assignment 4
 *******************************************************************************/


#include "main.h"
#include "stm32l476xx.h"

#define SPI_PIN 7

void spi_init(){

	// configure MOSI, MISO, SCK pins
	GPIOA->MODER &= ~(0x3U << SPI_PIN * 2);
	GPIOA->MODER |= (0x2U << SPI_PIN * 2);



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
