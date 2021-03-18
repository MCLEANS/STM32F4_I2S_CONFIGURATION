#include "stm32f4xx.h"
#include "clockconfig.h"

custom_libraries::clock_config system_clock;

int main(void) {
  
  system_clock.initialize();

  /* Enable clock port RCC */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  /* Enable SPI RCC */
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

  /* Set clock pin to output Alternate Function */
  GPIOB->MODER &= ~GPIO_MODER_MODER10;
  GPIOB->MODER |= GPIO_MODER_MODER10_1;

  /* Select specific alternate function */
  GPIOB->AFR[1] |= (5<<8);

  /* Enable PLLI2S */
  RCC->CR |= RCC_CR_PLLI2SON;

  /* Wait until PLLI2S is Enabled */
  while(!(RCC->CR & RCC_CR_PLLI2SRDY)){}

  /* PLL clock source automatically set to HSI by Clock configuration driver */
  
  /* Select I2S clock source to PLLI2S */
  RCC->CFGR &= ~RCC_CFGR_I2SSRC;

  /**
   * Configure the I2S Clock frequency
   * PLL I2S Clock output frequency is set at 86MHz
   */
  
  /* Set PLLI2SN value to 258 */
  RCC->PLLI2SCFGR &= ~RCC_PLLI2SCFGR_PLLI2SN;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SN_1;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SN_2;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SN_3;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SN_5;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SN_8;

  /* Set PLLI2SR to 3 */
  RCC->PLLI2SCFGR &= ~RCC_PLLI2SCFGR_PLLI2SR;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SR_0;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SR_1;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SR_2;

  /* Set the I2S Linear prescaler with a Value of 42 */
  SPI2->I2SPR &= ~SPI_I2SPR_I2SDIV;
  SPI2->I2SPR |= 220;

  /* Set odd factor for the I2S prescaler */
  SPI2->I2SPR &= ~SPI_I2SPR_ODD;

  /* Enable I2S mode */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SMOD;

  /* Set I2S to master receive mode */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SCFG;

 // SPI2->I2SPR |= SPI_I2SPR_MCKOE;

  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SSTD_1;

  /* Enable SPI */
  SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;








  while(1){
  

  }
}
