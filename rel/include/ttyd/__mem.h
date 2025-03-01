#pragma once

#include <cstdint>

namespace ttyd::__mem
{
    extern "C"
    {
        void *memset(void *pointer, uint32_t value, uint32_t amountOfBytesToSet);
        // __fill_mem
        // memcpy
    }
} // namespace ttyd::__mem
