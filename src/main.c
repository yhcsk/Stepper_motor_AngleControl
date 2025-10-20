#include "board.h"
#include "sysclock.h"
#include "dwt_delay.h"
#include "uart_io.h"
#include "stepper.h"

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  Stepper_Init();
  MX_USART3_UART_Init();
  DWT_Delay_Init();

  /* On power-up: disable → enable after a delay */
  TMC_Enable(false);
  HAL_Delay(20);
  TMC_Enable(true);
  HAL_Delay(10);

  uart_print("\r\n=== NUCLEO-F767ZI Stepper (STEP/DIR) ===\r\n");
  uart_printf("FULL=%u, uStep=%u, step_period=%uus\r\n",
              FULL_STEPS_PER_REV, MICROSTEPS, STEP_PERIOD_US);
#if ALLOW_ONLY_FOUR_ANGLES
  uart_print("Mode: ONLY {±30, ±45, ±90, ±180} degrees\r\n");
#else
  uart_print("Mode: ANY angle (positive=CW, negative=CCW)\r\n");
#endif
  uart_print("Enter angle and press <Enter> (e.g. 30, -45, 90.0):\r\n");

  char line[48];
  while (1) {
    uart_print("> ");
    int n = uart_readline(line, sizeof(line));
    if (n <= 0) continue;

    /* Trim leading whitespace */
    char *p = line;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\0') continue;

    /* Support simple commands: help, ?, set, get, etc. */
    if (strcasecmp(p, "help") == 0 || strcmp(p, "?") == 0) {
#if ALLOW_ONLY_FOUR_ANGLES
      uart_print("Input one of: 30, -30, 45, -45, 90, -90, 180, -180\r\n");
#else
      uart_print("Type any angle (e.g. 12.5, -90). One move per enter.\r\n");
#endif
      continue;
    }

    /* Parse input as a float angle */
    char *endptr = NULL;
    float deg = strtof(p, &endptr);
    if (endptr == p) {
      uart_print("ERR: not a number. Type e.g. 30 or -45\r\n");
      continue;
    }

    uart_printf("Move %.3f deg ...\r\n", deg);
    Stepper_Move_Degrees_Once(deg);
    uart_print("Done.\r\n");
  }
}

/* Optional Error_Handler (for link completeness) */
void Error_Handler(void) {
  __disable_irq();
  while (1) {}
}
