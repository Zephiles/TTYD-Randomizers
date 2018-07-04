#pragma once

namespace ttyd::evtmgr {

extern "C" {

// evtGetPtrID
// evtGetPtr
// evtStartOther
// evtStopOther
// evtStartAll
// evtStopAll
// evtStartID
// evtStopID
// evtStart
// evtStop
// evtSetType
// evtSetSpeed
// evtSetPri
bool evtCheckID(uint32_t threadID);
// evtDeleteID
// evtDelete
// evtmgrMain
// evtRestart
// evtBrotherEntry
// evtChildEntry
uint32_t evtEntryType(void *script, uint32_t unk1, uint32_t unk2, uint32_t unk3);
// evtEntry
// evtEntryRunCheck
// evtmgrReInit
// evtmgrInit
// make_pri_table
// evtGetWork

}

}