#pragma once

namespace ttyd::icondrv {

extern "C" {

// iconNumberDispGx3D
// iconNumberDispGx
// iconSetAlpha
// iconSetScale
// iconFlagOff
// iconFlagOn
// iconSetPos
// iconNameToPtr
// iconGX
// iconGetWidthHight
// iconGetTexObj
// iconDispGxCol
// iconDispGx2
void iconDispGx(float position[3], uint16_t unkBits, uint16_t iconNum, float scale);
void iconDispGxAlpha(float position[3], uint16_t unkBits, uint16_t iconNum, uint8_t alpha, float scale);
// iconDisp
// iconChange
// iconDelete
// iconEntry2D
// iconEntry
// iconMain
// iconReInit
// iconTexSetup
// iconInit
// _callback_bin
// _callback_tpl

}

}