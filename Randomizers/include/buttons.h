#pragma once

#include <cstdio>

// Enumerations
enum PAD_INPUTS : uint16_t
{
  PAD_DPAD_LEFT = 0x0001,
  PAD_DPAD_RIGHT = 0x0002,
  PAD_DPAD_DOWN = 0x0004,
  PAD_DPAD_UP = 0x0008,
  PAD_Z = 0x0010,
  PAD_R = 0x0020,
  PAD_L = 0x0040,
  PAD_A = 0x0100,
  PAD_B = 0x0200,
  PAD_X = 0x0400,
  PAD_Y = 0x0800,
  PAD_START = 0x1000,
};