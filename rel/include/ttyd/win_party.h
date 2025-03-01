#pragma once

#include <cstdint>

namespace ttyd::win_party
{
    extern "C"
    {
        // winPartyDisp
        // winPartyMain2
        // winPartyMain
        void winPartyExit(void *pauseMenuPointer);
        // winPartyInit2
        void winPartyInit(void *pauseMenuPointer);
    }
} // namespace ttyd::win_party
