#ifndef STEPPER_H
#define STEPPER_H

#include "board.h"
#include "dwt_delay.h"

/* Initialize STEP/DIR/EN GPIOs and default levels */
void Stepper_Init(void);

/* Low level = enable */
static inline void TMC_Enable(bool on) {
  HAL_GPIO_WritePin(ENN_GPIO_PORT, ENN_PIN, on ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/* Direction: true = CW, false = CCW.
   If the motor rotates in the opposite direction, simply invert this logic. */
static inline void Stepper_SetDir(bool cw) {
  HAL_GPIO_WritePin(DIR_GPIO_PORT, DIR_PIN, cw ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Send one STEP pulse */
static inline void Stepper_PulseOnce(void) {
  HAL_GPIO_WritePin(STEP_GPIO_PORT, STEP_PIN, GPIO_PIN_SET);
  delay_us(STEP_PULSE_US);
  HAL_GPIO_WritePin(STEP_GPIO_PORT, STEP_PIN, GPIO_PIN_RESET);
}

/* Convert arbitrary angle → absolute step count (always positive). */
uint32_t degrees_to_steps_abs(float deg_abs);

/* Check if an angle is allowed (when ALLOW_ONLY_FOUR_ANGLES == 1). */
bool angle_allowed(float deg);

/* Perform a single move: executes only once per input. */
void Stepper_Move_Degrees_Once(float deg);

#endif /* STEPPER_H */
