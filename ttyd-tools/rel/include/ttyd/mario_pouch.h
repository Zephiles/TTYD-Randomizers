#pragma once

namespace ttyd::mario_pouch {

extern "C" {

// pouchGetYoshiName
// pouchSetYoshiName
// pouchSetPartyColor
// pouchGetPartyColor
// pouchCheckMail
// pouchOpenMail
// pouchReceiveMail
// pouchReceiveMailCount
// pouchGetStarStone
// pouchAddKpaScore
// pouchGetKpaScore
// pouchAddKpaCoin
// pouchGetKpaCoin
// pouchMajinaiInit
// pouchArriveBadge
// unk_JP_US_PAL_049
// pouchEquipBadgeID
// pouchEquipCheckBadgeIndex
// pouchEquipCheckBadge
// pouchUnEquipBadgeIndex
// pouchEquipBadgeIndex
// pouchGetStarPoint
// pouchRevisePartyParam
void pouchReviseMarioParam();
// pouchRemoveKeepItem
// pouchAddKeepItem
// pouchGetPartyAttackLv
// pouchGetHammerLv
// pouchGetJumpLv
// pouchSetAudienceNum
// pouchGetAudienceNum
// pouchGetMaxAP
// pouchSetAP
// pouchAddAP
// pouchGetAP
// pouchSetMaxFP
// pouchSetFP
// pouchGetMaxFP
// pouchGetFP
// pouchSetPartyHP
// pouchGetPartyHP
// pouchSetMaxHP
// pouchSetHP
// pouchGetMaxHP
// pouchGetHP
// pouchAddHP
// pouchAddStarPiece
// pouchGetStarPiece
// pouchSetSuperCoin
// pouchGetSuperCoin
// pouchSetCoin
// pouchAddCoin
// pouchGetCoin
// pouchSortItem
// comp_kind_r
// comp_aiueo_r
// comp_kind
// comp_aiueo
// unk_JP_US_PAL_050
// pouchRemoveItemIndex
// pouchRemoveItem
uint32_t pouchCheckItem(uint32_t id);
// pouchGetItem
// pouchGetEmptyKeepItemCnt
// pouchGetEmptyHaveItemCnt
// pouchGetEquipBadgeCnt
uint32_t pouchGetHaveBadgeCnt();
// pouchGetKeepItemCnt
// pouchGetHaveItemCnt
// pouchHaveBadge
// pouchKeepItem
// pouchHaveItem
// pouchKeyItem
// pouchInit
void *pouchGetPtr();

}

}