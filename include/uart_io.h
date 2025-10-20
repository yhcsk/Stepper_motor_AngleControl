#ifndef UART_IO_H
#define UART_IO_H

#include "board.h"

/* Global UART handle (USART3) */
extern UART_HandleTypeDef huart3;
#define HUART huart3

void MX_USART3_UART_Init(void);

/* Simple UART I/O */
void uart_print(const char *s);
void uart_printf(const char *fmt, ...);

/* Read one line (terminated by '\n'), with echo and backspace support.
   Returns the number of characters read (excluding '\0'). */
int uart_readline(char *buf, int maxlen);

#endif /* UART_IO_H */
