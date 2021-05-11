#include "stm32f4xx.h"
#include "clockconfig.h"
#include "USART.h"
#include <stdlib.h>

custom_libraries::clock_config system_clock;
custom_libraries::USART serial(USART1,GPIOB,7,6,9600);

uint16_t received_data = 17;

int main(void) {
  
  system_clock.initialize();

  /* Enable clock port RCC */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  /* Enable data pin RCC */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  /* Enable SPI RCC */
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  /* enable DMA 1 */
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
  /* Set clock pin to output Alternate Function */
  GPIOB->MODER &= ~GPIO_MODER_MODER10;
  GPIOB->MODER |= GPIO_MODER_MODER10_1;
  /* Set GPIO SPEED to very high */
  GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10_1;
  /* Select specific alternate function */
  GPIOB->AFR[1] |= (5<<8);
  /* set I2S data pin to alternate function */
  GPIOC->MODER &= ~GPIO_MODER_MODER3;
  GPIOC->MODER |= GPIO_MODER_MODER3_1;
  /* Set PIN to high speed */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;
  /* Select the specific alternate function */
  GPIOC->AFR[0] |= (5<<12);

  /* PLL clock source automatically set to HSI by Clock configuration driver */
  /* Select I2S clock source to PLLI2S */
  RCC->CFGR &= ~RCC_CFGR_I2SSRC;
  /**
   * Configure the I2S Clock frequency
   * PLL I2S Clock output frequency is set at 86MHz
   */
  /* Set PLLI2SN value to 258 */
  RCC->PLLI2SCFGR &= ~RCC_PLLI2SCFGR_PLLI2SN;
  RCC->PLLI2SCFGR |= (258 << 6);
  /* Set PLLI2SR to 3 */
  RCC->PLLI2SCFGR &= ~RCC_PLLI2SCFGR_PLLI2SR;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SR_0;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SR_1;
  /* Enable PLLI2S */
  RCC->CR |= RCC_CR_PLLI2SON;
  /* Wait until PLLI2S is Enabled */
  while(!(RCC->CR & RCC_CR_PLLI2SRDY)){}

  /* Disable I2S */
  SPI2->I2SCFGR &= ~SPI_I2SCFGR_I2SE;
  /* Set the I2S Linear prescaler with a Value of 42 */
  SPI2->I2SPR &= ~SPI_I2SPR_I2SDIV;
  SPI2->I2SPR |= 42;
  /* Set odd factor for the I2S prescaler */
  SPI2->I2SPR &= ~SPI_I2SPR_ODD;
  /* Select I2S mode */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SMOD;
  /* Set I2S to master receive mode */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SCFG;
  /* Set MSB Justified standard */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SSTD_0;
  /* Set CPOL to LOW */
  SPI2->I2SCFGR &= ~SPI_I2SCFGR_CKPOL;
  /* Set data length to 16 bit */
  SPI2->I2SCFGR &= ~SPI_I2SCFGR_DATLEN;
  /* set channel length to 16 bits */
  SPI2->I2SCFGR &= ~SPI_I2SCFGR_CHLEN;
  /* Enable RX DMA capability */
  SPI2->CR2 |= SPI_CR2_RXDMAEN;
  /* Enable I2S */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;

 // DMA1_Stream3->CR &= ~DMA_SxCR_EN;

  /* Select stream 3 channel 0 of DMA */
 // DMA1_Stream3->CR &= ~DMA_SxCR_CHSEL;
/*
	//set Memory data size to 16 bits
	DMA1_Stream3->CR |= DMA_SxCR_MSIZE_0;
	//Set peripheral data size to 16 bits
	DMA1_Stream3->CR |= DMA_SxCR_PSIZE_0; 
	//set memory increment mode
	DMA1_Stream3->CR |= DMA_SxCR_MINC;
	//Enable DMA circular mode
	DMA1_Stream3->CR |= DMA_SxCR_CIRC;
	//Number of data Items to be transfered
	DMA1_Stream3->NDTR = 1;
	//Give peripheral address
	DMA1_Stream3->PAR = (uint32_t)(&SPI2->DR);
	//Give memory address
	DMA1_Stream3->M0AR = (uint32_t)(&received_data);
	//Enable the DMA
	DMA1_Stream3->CR |= DMA_SxCR_EN;
*/
  /* Initialize USART */
  serial.initialize();
  
  while(1){
    char data[5];
    while(!(SPI2->SR & SPI_SR_RXNE)){}
    if(!(SPI2->SR & SPI_SR_CHSIDE)){
      itoa(SPI2->DR,data,10);
      serial.println(data);
    }
  }
}
