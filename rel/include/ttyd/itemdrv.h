#pragma once

namespace ttyd::itemdrv {

extern "C" {

// itemPickUp
// itemStatus
// itemStatusOn
// itemFlag
// itemFlagOff
// itemFlagOn
// itemNokoForceGet
// itemForceGet
// itemHitCheckSide
// itemSetPosition
// itemseq_Bound
// itemseq_GetItem
// winFullDisp
// winHelpDisp
// winHelpMain
// winNameDisp2
// winNameDisp
// itemNearDistCheck
// itemHitCheck
// itemNameToPtr
// itemDelete
void *itemEntry(const char *name, uint32_t id, uint32_t mode, int32_t wasCollectedExpr, void *pickupScript, float coordinateX, float coordinateY, float coordinateZ);
// itemModeChange
// itemMain
// unk_JP_US_PAL_023
// itemGetNokoCheck
// itemGetCheck
// itemCoinDrop
// itemReInit
// itemInit

}

}