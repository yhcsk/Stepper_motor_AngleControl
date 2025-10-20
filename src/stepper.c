#include "stepper.h"

/* Used only when "allow four specific angles" mode is enabled */
static const float kAllowedAnglesDeg[] = {30.0f, 45.0f, 90.0f, 180.0f};

void Stepper_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Initial levels: ENN=high (disabled), DIR/STEP=low */
  HAL_GPIO_WritePin(ENN_GPIO_PORT, ENN_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(DIR_GPIO_PORT, DIR_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(STEP_GPIO_PORT, STEP_PIN, GPIO_PIN_RESET);

  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = ENN_PIN;
  HAL_GPIO_Init(ENN_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DIR_PIN;
  HAL_GPIO_Init(DIR_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = STEP_PIN;
  HAL_GPIO_Init(STEP_GPIO_PORT, &GPIO_InitStruct);
}

/* Convert arbitrary angle → absolute step count. Negative angles handled by caller. */
uint32_t degrees_to_steps_abs(float deg_abs) {
  float steps_f = (deg_abs / 360.0f) * (float)FULL_STEPS_PER_REV * (float)MICROSTEPS;
  uint32_t steps = (uint32_t)lroundf(steps_f);
  return steps;
}

/* Check if angle is allowed (only if restriction mode is on). */
bool angle_allowed(float deg) {
#if ALLOW_ONLY_FOUR_ANGLES
  float x = fabsf(deg);
  for (size_t i = 0; i < sizeof(kAllowedAnglesDeg)/sizeof(kAllowedAnglesDeg[0]); ++i) {
    if (fabsf(x - kAllowedAnglesDeg[i]) < 0.01f) return true;
  }
  return false;
#else
  (void)deg;
  return true;
#endif
}

/* Perform a single move: executes only once per input. */
void Stepper_Move_Degrees_Once(float deg) {
  if (!angle_allowed(deg)) {
    uart_print("ERR: angle not allowed in current mode. ");
#if ALLOW_ONLY_FOUR_ANGLES
    uart_print("Allowed: 30, 45, 90, 180 (±).\r\n");
#else
    uart_print("Please input any numeric angle (e.g. 30, -45, 90.0)\r\n");
#endif
    return;
  }

  bool dir_cw = (deg >= 0.0f);
  float deg_abs = fabsf(deg);
  uint32_t steps = degrees_to_steps_abs(deg_abs);
  if (steps == 0) {
    uart_print("WARN: 0 step, ignored.\r\n");
    return;
  }

  Stepper_SetDir(dir_cw);
  delay_us(5);

  for (uint32_t i = 0; i < steps; ++i) {
    Stepper_PulseOnce();
    delay_us(STEP_PERIOD_US - STEP_PULSE_US);
  }
}
