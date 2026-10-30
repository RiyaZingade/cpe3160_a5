/*******************************************************************************
 * @file           : main.c
 * @brief          : Main program body for assignment 5
 *******************************************************************************/


#include "main.h"
#include "stm32l476xx.h"

#define SPI_MOSI 7  // PA7
#define SPI_SCK 5   // PA5
#define SPI_CS 4    // PA4 for chip select

void DAC_init(){

	// configure MOSI, MISO, SCK pins, setting GPIOs to Alternate function mode (SPI)

	//telling STM32 that we want peripheral to control PA7, not normal GPIO software
	GPIOA->MODER &= ~(0x3U << SPI_MOSI * 2);
	GPIOA->MODER |= (0x2U << SPI_MOSI * 2);

	// telling STM32 that we want peripheral to control PA5, not GPIO software 
	GPIOA->MODER &= ~(0x3U << SPI_SCK * 2);
	GPIOA->MODER |= (0x2U << SPI_SCK * 2);

	//telling STM32 WHICH peripheral function should control PA7 --> PA7 uses AF5 for SPI1_MOSI
	GPIOA->AFR[0] &= ~(0xFU << SPI_MOSI * 4); //clears all 4 pins in that area
	GPIOA->AFR[0] |= (0x5U << SPI_MOSI * 4); // setting the 4 pins to 0101

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
	SPI1->CR1 |= (1U << 15);
	SPI1->CR1 |= (1U << 14);


	// Initialize CS HIGH (DAC deselected)
	GPIOA->ODR |= (1U << SPI_CS);
	
	// setting pa4 to output mode (not alt func because we are manually controlling it rather than a peripheral)
	// 0 --> means DAC is selected and 1 --> means DAC is deselected 
	GPIOA->MODER &= ~(0x3U << SPI_CS * 2);
	GPIOA->MODER |= (1U << SPI_CS * 2);
	
	// Clear DS[3:0]
	SPI1->CR2 &= ~(0xF << 8);
	// Set DS[3:0] for 16-bit frames
	SPI1->CR2 |= (0xF << 8);

	// Enable SPI1 after configuration
	SPI1->CR1 |= (1U << 6);
}

int DAC_write(uint16_t input){
    if (input > 4095) {
        return -1;
    }
    // DAC takes 16-bit command, top 4 bits are control bits
    // bottom 12-bits: write value
    uint16_t command = (input & 0x0FFF) | 0x3000;
    // Pull CS LOW, Select DAC
    GPIOA->BSRR = (1 << (SPI_CS + 16));
    // Wait for TXE
    while (!(SPI1->SR & SPI_SR_TXE)) { }
    // write command to data register
    SPI1->DR = command;
    // wait for TXE
    while (!(SPI1->SR & SPI_SR_TXE)) { }
    // wait until SPI (bit 7) is not busy
    while (SPI1->SR & SPI_SR_BSY) { }
    // Pull CS HIGH, Deselect DAC
    GPIOA->BSRR = (1 << SPI_CS);
    return 0;
}

uint16_t DAC_volt_conv(uint16_t voltage_mV) {
    if (voltage_mV > 3300) {
        return 0;
    }
    uint32_t result = (uint32_t)voltage_mV * 4096 / 3300;
    if (result >= 4096) {
        result = 4095;
    }
    return result;
}


int main(void)
{
	HAL_Init();
	// SystemClock_Config();
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN); // Enable GPIOA clock
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN); //enable SPI clock 
	
	DAC_init();

	uint16_t high_V = DAC_volt_conv(2000);
	uint16_t low_V = DAC_volt_conv(1000);
	DAC_write(high_V);
	// while(1) {
	//     DAC_write(high_V);
	// 	HAL_Delay(25);
	// 	DAC_write(low_V);
	// 	HAL_Delay(75);  
	// }

 }
