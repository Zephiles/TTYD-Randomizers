#pragma once

#include <cstdint>

namespace ttyd::mot_ship
{
    extern "C"
    {
        // checkHarbor
        // shipMove
        // marioChkShipMoveMode
        // marioShipChgFall
        // mot_ship
        // mot_ship_post
        void marioReInit_ship();
        // marioForceShipAnime
        // marioChkShipPlace
        // marioShipSetDispDir
        void *marioShipForceStop();
        // marioChkSwirlPower
        // marioSetSwirlPower
        // marioShipChanging
    }
} // namespace ttyd::mot_ship
