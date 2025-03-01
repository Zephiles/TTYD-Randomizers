#pragma once

#include <cstdint>

namespace ttyd::mario_pouch
{
    extern "C"
    {
        // pouchGetYoshiName
        // pouchSetYoshiName
        // pouchSetPartyColor
        // pouchGetPartyColor
        // pouchCheckMail
        // pouchOpenMail
        // pouchReceiveMail
        // pouchReceiveMailCount
        void pouchGetStarStone(uint32_t id);
        // pouchAddKpaScore
        // pouchGetKpaScore
        // pouchAddKpaCoin
        // pouchGetKpaCoin
        // pouchMajinaiInit
        // pouchArriveBadge
        // pouchUnEquipBadgeID
        // pouchEquipBadgeID
        // pouchEquipCheckBadgeIndex
        uint32_t pouchEquipCheckBadge(uint32_t id);
        // pouchUnEquipBadgeIndex
        // pouchEquipBadgeIndex
        // pouchGetStarPoint
        void pouchRevisePartyParam();
        void pouchReviseMarioParam();
        // pouchRemoveKeepItem
        // pouchAddKeepItem
        int16_t pouchGetPartyAttackLv(uint32_t partnerId);
        uint32_t pouchGetHammerLv();
        uint32_t pouchGetJumpLv();
        // pouchSetAudienceNum
        // pouchGetAudienceNum
        int16_t pouchGetMaxAP();
        void pouchSetAP(int16_t sp);
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
        int16_t pouchGetStarPiece();
        // pouchSetSuperCoin
        // pouchGetSuperCoin
        // pouchSetCoin
        // pouchAddCoin
        int16_t pouchGetCoin();
        // pouchSortItem
        // comp_kind_r
        // comp_aiueo_r
        // comp_kind
        // comp_aiueo
        // pouchRemoveItemIndex
        // pouchRemoveItem
        uint32_t pouchCheckItem(uint32_t id);
        bool pouchGetItem(uint32_t id);
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
} // namespace ttyd::mario_pouch
