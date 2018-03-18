// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <detector/key_input_detector.h>
#include <detector/event_detector.h>
#include <detector/timeout_detector.h>
#include <led_driver.h>
#include <utils/logger.h>

#define KEYBOARD_DEVICE "/dev/input/event2"
#define LED_DEVICE      "/sys/class/leds/input2::capslock/brightness"

int main(void) {
  struct timeval time = {};
  const struct input_event kPressA = {time, EV_KEY, KEY_A, INPUT_KEY_PRESSED};
  EventDetector press_a = CreateKeyInputDetector(KEYBOARD_DEVICE, &kPressA);
  if (StartEventDetector(press_a) != DETECTOR_SUCCESS) {
    DEBUG_LOG("Fail to init input device\n");
    exit(1);
  }

  EventDetector five_sec_timer = CreateTimeOutDetector(5000, TIMER_ONE_SHOT);
  
  LedDriver caps_led = CreateLedDriver();
  if (InitLedDriver(caps_led, LED_DEVICE) != LED_DRIVER_SUCCESS) {
    DEBUG_LOG("Fail to init led device\n");
    exit(1);
  }

  while(CheckEvent(press_a) != DETECTOR_EVENT_DETECTED) {}

  TurnOnLed(caps_led);

  StartEventDetector(five_sec_timer);
  while(CheckEvent(five_sec_timer) != DETECTOR_EVENT_DETECTED) {}
  TurnOffLed(caps_led);

  CleanupEventDetector(press_a);
  DestroyKeyInputDetector(press_a);
  CleanupEventDetector(five_sec_timer);
  DestroyKeyInputDetector(five_sec_timer);

  CleanupLedDriver(caps_led);
  DestroyLedDriver(caps_led);

  return 0;
}