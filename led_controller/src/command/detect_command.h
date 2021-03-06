// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef LED_CONTROLLER_COMMAND_DETECT_COMMAND_H_
#define LED_CONTROLLER_COMMAND_DETECT_COMMAND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <command/command.h>

Command CreateKeyInputDetectCommand();
void DestroyKeyInputDetectCommand(Command cmd);

#ifdef __cplusplus
}
#endif

#endif  // LED_CONTROLLER_COMMAND_DETECT_COMMAND_H_
