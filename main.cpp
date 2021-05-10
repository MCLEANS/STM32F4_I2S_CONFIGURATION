#include "stm32f4xx.h"
#include "clockconfig.h"

custom_libraries::clock_config system_clock;

#define DFREQ 12000
#define FS DFREQ
#define DECIMATION 64
#define I2S_FS ((FS*DECIMATION)/(16*2))

int main(void) {
  
  system_clock.initialize();

  /* Enable clock port RCC */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  /* Enable data pin RCC */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  /* Enable SPI RCC */
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

  /* Set clock pin to output Alternate Function */
  GPIOB->MODER &= ~GPIO_MODER_MODER10;
  GPIOB->MODER |= GPIO_MODER_MODER10_1;

  /* Set GPIO SPEED to veery high */
  GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR10;
  GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;

  /* Select specific alternate function */
  GPIOB->AFR[1] |= (5<<8);


  /* set I2S data pin to alternate function */
  GPIOC->MODER &= ~GPIO_MODER_MODER3;
  GPIOC->MODER |= GPIO_MODER_MODER3_1;

  /* Select the specific alternate function */
  GPIOC->AFR[0] |= (5<<12);

  /* Set PIN to high speed */
  GPIOC->MODER &= ~GPIO_OSPEEDER_OSPEEDR3;
  GPIOC->MODER |= GPIO_OSPEEDER_OSPEEDR3_1;


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

  /* Set the I2S Linear prescaler with a Value of 42 */
  SPI2->I2SPR &= ~SPI_I2SPR_I2SDIV;
  SPI2->I2SPR |= 42;

  /* Set odd factor for the I2S prescaler */
  SPI2->I2SPR &= ~SPI_I2SPR_ODD;

  /* Select I2S mode */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SMOD;

  /* Set I2S to master receive mode */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SCFG;


  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SSTD_1;

  SPI2->I2SCFGR |= SPI_I2SCFGR_CKPOL;

  SPI2->I2SCFGR &= ~SPI_I2SCFGR_DATLEN;

  /* Enable SPI */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;

  while(1){
  

  }
}
