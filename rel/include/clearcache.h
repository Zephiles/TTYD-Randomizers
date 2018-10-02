#pragma once

#include <cstdint>

namespace clearcache {

void clear_DC_IC_Cache(void *ptr, uint32_t size);

}