#pragma once

namespace ttyd::swdrv {

extern "C" {

// _swByteGet
// _swByteSet
// _swClear
// _swGet
// _swSet
uint32_t swByteGet(uint32_t gsw);
void swByteSet(uint32_t gsw, uint32_t value);
void swClear(uint32_t gswf);
bool swGet(uint32_t gswf);
void swSet(uint32_t gswf);
// swReInit
// swInit

}

}