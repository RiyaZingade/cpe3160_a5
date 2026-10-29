/*******************************************************************************
 * @file           : main.c
 * @brief          : Main program body for assignment 5
 *******************************************************************************/


#include "main.h"
#include "stm32l476xx.h"

#define SPI_MOSI 7
#define SPI_SCK 5
#define SPI_CS 4 // for chip select

void DAC_init(){

	// configure MOSI, MISO, SCK pins

	//telling STM32 that we want peripheral to control PA7, not normal GPIO software
	GPIOA->MODER &= ~(0x3U << SPI_PIN * 2);
	GPIOA->MODER |= (0x2U << SPI_PIN * 2);

	// telling STM32 that we want peripheral to control PA5, not GPIO software 
	GPIOA->MODER &= ~(0x3U << SPI_SCK * 2);
	GPIOA->MODER |= (0x2U << SPI_SCK * 2);

	//telling STM32 WHICH peripheral function should control PA7 --> PA7 uses AF5 for SPI1_MOSI
	GPIOA->AFR[0] &= ~(0xFU << SPI_PIN * 4); //clears all 4 pins in that area
	GPIOA->AFR[0] |= (0x5U << SPI_PIN * 4); // setting the 4 pins to 0101

	//telling STM32 which periferal function should control PA5 --> PA5 uses AF5 for SPI1_SCk
	GPIOA->AFR[0] &= ~(0xFU << SPI_SCK * 4);
	GPIOA->AFR[0] |= (0x5U << SPI_SCK * 4);

	
	// configuring CR1 for SPI1
	//setting STM32 board to be the master (stm is generating the clock signal --> SCK)
	SPI1->CR1 |= (1U << 2);
	SPI1->CR1 &= ~(1U << 1); //CPOL --> clock polarity
	SPI1->CR1 &= ~(1U << 0); // CPHA --> clock phase
	SPI1->CR1 &= ~(1U << 7); // LSBFIRST --> MCP4921 expectes 16bit command to be MSB first, so this value will be 0
	SPI1->CR1 &= ~(7U << 3); // setting BR[2:0] to 8 so we can divide clock by 8 
	SPI1->CR1 |= (2U << 3); // BR is Buade Rate which is a clock prescaler that controls communication speed
	SPI1->CR1 |= (1U << 9); // bit 9 is SSM --> software slave management
	SPI1->CR1 |= (1U << 8); // bit 8 is SSI --> internal slave select 

	// setting pa4 to output mode (not alt func because we are manually controlling it rather than a peripheral)
	// 0 --> HIGH means DAC is selected and 1 --> means DAC is deselected 
	GPIOA->MODER &= ~(0x3U << SPI_CS * 2);
	GPIOA->MODER |= (1U << SPI_CS * 2);

}

int DAC_write(uint16_t input){
    // DAC takes 16-bit command, top 4 bits are control bits
    // bottom 12-bits: write value
    uint16_t command = (input & 0x0FFF) | 0x3000;

    
    
}

uint16_t DAC_volt_conv(uint16_t voltage_mV) {
    return voltage_mV * 4096 / 3300
}


int main(void)
{
	HAL_Init();
	// SystemClock_Config();
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN); // Enable GPIOA clock
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN); //enable SPI clock 
	
	DAC_init();


	while(1) {

	}

 }
