#pragma once

namespace ttyd::swdrv {

extern "C" {

// _swByteGet
// _swByteSet
// _swClear
// _swGet
// _swSet
uint32_t swByteGet(uint32_t gswOffset);
void swByteSet(uint32_t gswOffset, uint32_t value);
// swClear
// swGet
// swSet
// swReInit
// swInit

}

}