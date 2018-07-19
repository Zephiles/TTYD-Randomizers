#pragma once

#include <cstdint>

namespace ttyd::mario_cam {

extern "C" {

// marioCamZoomUpLevelMain
// marioCamZoomUp
// marioCamZoomOff
// marioCamZoomOffReq2
// marioCamZoomOffReq
// camFollowYOff
// camFollowYOn
// marioNoUpdateCamPos
// marioUpdateCamPos
// marioUpdateCam
// marioUpdateCamCollision
// marioCamMain
// marioPreCamera
// marioGetCamFollowRate
// marioSetCamFollowRate
// marioResetCamFollowRate
void marioSetCamId(uint32_t id);
// marioGetCamId
// marioInitCamId
// marioResetCamShiftRate
// marioResetCamShift

}

}
