// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <detector/event_detector.h>

int StartEventDetector(EventDetector detector) {
  if (detector == NULL) return DETECTOR_ERROR;
  return detector->vtable->Start(detector);
}

int CheckEvent(EventDetector detector) {
  if (detector == NULL) return DETECTOR_ERROR;
  return detector->vtable->CheckEvent(detector);
}

int CleanupEventDetector(EventDetector detector) {
  if (detector == NULL) return DETECTOR_ERROR;
  return detector->vtable->Cleanup(detector);
}
