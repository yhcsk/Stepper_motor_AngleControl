#include "uart_io.h"

UART_HandleTypeDef huart3;

/* USART3 init: PD8 = TX, PD9 = RX, AF7, 115200 8N1 */
void MX_USART3_UART_Init(void)
{
  __HAL_RCC_USART3_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* PD8 = USART3_TX, PD9 = USART3_RX */
  GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  huart3.Instance          = USART3;
  huart3.Init.BaudRate     = UART_BAUD;
  huart3.Init.WordLength   = UART_WORDLENGTH_8B;
  huart3.Init.StopBits     = UART_STOPBITS_1;
  huart3.Init.Parity       = UART_PARITY_NONE;
  huart3.Init.Mode         = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK) {
    while (1) {}
  }
}

/* Simple UART print */
void uart_print(const char *s) {
  HAL_UART_Transmit(&HUART, (uint8_t*)s, (uint16_t)strlen(s), HAL_MAX_DELAY);
}

void uart_printf(const char *fmt, ...) {
  char buf[128];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  uart_print(buf);
}

/* Read one line (terminated by '\n'), with echo and backspace support. */
int uart_readline(char *buf, int maxlen) {
  int idx = 0;
  while (1) {
    uint8_t ch;
    if (HAL_UART_Receive(&HUART, &ch, 1, HAL_MAX_DELAY) != HAL_OK) continue;

    if (ch == '\r') continue;            /* Ignore CR */
    if (ch == '\n') {                    /* End of line */
      uart_print("\r\n");
      buf[idx] = '\0';
      return idx;
    }
    if ((ch == 0x08 || ch == 0x7F) && idx > 0) {  /* Backspace */
      idx--; buf[idx] = '\0';
      uart_print("\b \b");
      continue;
    }
    if (idx < maxlen - 1) {
      buf[idx++] = (char)ch;
      HAL_UART_Transmit(&HUART, &ch, 1, HAL_MAX_DELAY); /* Echo */
    }
  }
}
