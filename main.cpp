#include "stm32f4xx.h"
#include "clockconfig.h"

custom_libraries::clock_config system_clock;

int main(void) {
  
  system_clock.initialize();

  /* Enable PLLI2S */
  RCC->CR |= (1 << RCC_CR_PLLI2SON);

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
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SN_8;

  /* Set PLLI2SR to # */
  RCC->PLLI2SCFGR &= ~RCC_PLLI2SCFGR_PLLI2SR;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SR_0;
  RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SR_1;

  







  while(1){
  

  }
}
