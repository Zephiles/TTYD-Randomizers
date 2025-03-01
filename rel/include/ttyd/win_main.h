#pragma once

#include <cstdint>

namespace ttyd::win_main
{
    extern "C"
    {
        // unk_JP_US_PAL_58
        // unk_JP_US_PAL_59
        // unk_JP_US_PAL_60
        // unk_JP_US_PAL_61
        // winLectureCheck
        // winLectureOn
        // winLectureKeyMask
        // unk_JP_US_PAL_62
        // unk_JP_US_PAL_63
        // winIconSet
        // winIconGrayInit
        // winIconInit
        // winTexSet_x2
        // winTexInit_x2
        // winTexSetRot
        // unk_JP_US_PAL_64
        // winTexSet
        // winTexInit
        // winFontSetLabel
        // unk_US_PAL_12
        // winFontSetEdge
        // zz_8017ee58_
        // winFontSetR
        // winFontSetPitch
        // winFontSet
        // winFontInit
        // winGhostDiaryChk
        // cam_r
        // unk_JP_US_PAL_65
        // party_color
        // famicom_check
        // itemUseFunc2
        // itemUseFunc
        // winDispKoopa
        // winDisp
        // winMain
        // winOpenDisable
        // winOpenEnable
        // winCheck
        // winReInit
        // winInit
        void *winGetPtr();
    }
} // namespace ttyd::win_main
