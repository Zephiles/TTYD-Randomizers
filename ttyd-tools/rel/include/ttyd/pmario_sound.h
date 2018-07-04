#pragma once

#include <cstdint>

namespace ttyd::pmario_sound {

extern "C" {

// psndENV_LPF
// psndENVOff_f_d
// psndENVOff
// psndENVOn_f_d
// psndENVOn
// psndENVMain
// psndENV_stop
// psndSFX_get_vol
// psndSFXChk
// psndSFXOff
// psndSFX_dist
// psndSFX_pos
// psndSFX_pit
// psndSFX_vol
// psndSFXOnEx_3D
// psndSFXOn_3D
// psndSFXOnVol
// psndSFXOn
// psndSFXOn_
// __psndSFXOn
// psndSFXMain
// psndBGMPlayTime
// psndBGMStartCheck
// psndBGMScope
// psndBGMChkSilent
// psndBGMChk
// psndBGMOff_f_d
// psndBGMOff
// psndBGMOn_f_d
// unk_JP_US_PAL_051
// psndBGMOn
// psndBGMMain
// psndBGM_rate
// unk_JP_US_PAL_052
// psndSFXAllOff
// psndMapChange
// psndGetFlag
void psndClearFlag(uint16_t unkBit);
// psndSetFlag
// psndSetReverb
// psndStopAllFadeOut
// psndSetPosDirListener
// psndPushGroup
// psndExit
// psndMainInt
// psndMain
// psndInit
// searchPSSFXList
// calc3D
// angleABf

}

}
