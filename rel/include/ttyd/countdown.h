#pragma once

#include <cstdint>

namespace ttyd::countdown {

extern "C" {

// countDownFlagOff
// countDownFlagOn
// countDownStop
// countDownEnd
// countDownGetStatus
// countDownDisp
// countDownMain
// countDownSetRestartTime
void countDownStart(uint32_t unk1, uint32_t unk2);
// countDownSaveReStart
// countDownInit

}

}
