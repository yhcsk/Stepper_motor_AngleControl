#ifndef BOARD_H
#define BOARD_H

#include "stm32f7xx_hal.h"
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* ===================== configuration ===================== */
/* Motor parameters */
#define FULL_STEPS_PER_REV     200U      // One full circle of steps
#define MICROSTEPS             8U        // Must match TMC2209 jumper/solder configuration (e.g. 1/16)
#define STEP_PULSE_US          2U        // STEP high pulse width (>=1us)
#define STEP_PERIOD_US         800U      // Step period (us), smaller = faster

/* Pin mapping */
#define STEP_GPIO_PORT         GPIOA
#define STEP_PIN               GPIO_PIN_8

#define DIR_GPIO_PORT          GPIOA
#define DIR_PIN                GPIO_PIN_9

#define ENN_GPIO_PORT          GPIOA
#define ENN_PIN                GPIO_PIN_10   // Active-low enable pin

/* UART (VCP on Nucleo): USART3 → PD8(TX), PD9(RX) */
#define UART_BAUD              115200

/* Mode: restrict to four angles only? 1 = restrict; 0 = allow any angle */
#define ALLOW_ONLY_FOUR_ANGLES 1
/* ===================================================== */

#endif /* BOARD_H */
