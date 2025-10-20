#ifndef DWT_DELAY_H
#define DWT_DELAY_H

#include "board.h"

/* DWT microsecond delay (available on F7/M7 cores) */
static inline void DWT_Delay_Init(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->LAR = 0xC5ACCE55;  // Unlock (needed on some M7)
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static inline void delay_us(uint32_t us) {
  uint32_t cycles = (SystemCoreClock / 1000000U) * us;
  uint32_t start = DWT->CYCCNT;
  while ((DWT->CYCCNT - start) < cycles) { __NOP(); }
}

#endif /* DWT_DELAY_H */

