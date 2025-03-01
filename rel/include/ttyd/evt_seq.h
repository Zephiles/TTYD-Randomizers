#pragma once

#include <cstdint>

namespace ttyd::evt_seq
{
    extern "C"
    {
        // evt_seq_wait
        uint32_t evt_seq_set_seq(void *ptr);
    }
} // namespace ttyd::evt_seq
