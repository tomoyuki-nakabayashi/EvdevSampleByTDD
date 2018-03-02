// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <os/mock_io.h>

MOCK_IO *mock_io {};

int IO_OPEN(const char* pathname, int flags) {
  return mock_io->IO_OPEN(pathname, flags);
}

int IO_CLOSE(int fd) {
  return mock_io->IO_CLOSE(fd);
}