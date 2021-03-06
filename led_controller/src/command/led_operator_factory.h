// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef LED_CONTROLLER_COMMAND_LED_OPERATION_FACTORY_H_
#define LED_CONTROLLER_COMMAND_LED_OPERATION_FACTORY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <command/command.h>
#include <drivers/led_driver.h>

enum {
  OP_LED_TURN_ON = 0,
  OP_LED_TURN_OFF = 1,
  OP_LED_TOGGLE = 2,
  OP_MAX_FACTORY_ID,
};

Command LedOperatorFactory(LedDriver driver, int32_t op_id);
void DestroyLedOperator(Command super);

#ifdef __cplusplus
}
#endif

#endif  // LED_CONTROLLER_COMMAND_LED_OPERATION_FACTORY_H_
