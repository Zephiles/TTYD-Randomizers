#include "mod.h"

#include <ttyd/system.h>
#include <ttyd/itemdrv.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/msgdrv.h>
#include <ttyd/seq_battle.h>
#include <ttyd/mario_party.h>
#include <ttyd/party.h>
#include <ttyd/countdown.h>
#include <ttyd/mot_ship.h>
#include <ttyd/fadedrv.h>
#include <ttyd/evt_bero.h>
#include <ttyd/fontmgr.h>

#include "patch.h"

#include <cstdint>

namespace mod
{
    Mod *gMod = nullptr;

    void main()
    {
        Mod *mod = new Mod();
        mod->init();
    }

    Mod::Mod() {}

    void Mod::init()
    {
        gMod = this;

        mPFN_makeKey_trampoline = patch::hookFunction(ttyd::system::makeKey, []() { gMod->updateEarly(); });

        // Item Rando
        mPFN_itemEntry_trampoline = patch::hookFunction(ttyd::itemdrv::itemEntry,
                                                        [](const char *itemName,
                                                           uint32_t itemId,
                                                           uint32_t itemMode,
                                                           int32_t wasCollectedExpr,
                                                           void *itemPickupScript,
                                                           float itemCoordinateX,
                                                           float itemCoordinateY,
                                                           float itemCoordinateZ)
                                                        {
                                                            return gMod->getRandomItem(itemName,
                                                                                       itemId,
                                                                                       itemMode,
                                                                                       wasCollectedExpr,
                                                                                       itemPickupScript,
                                                                                       itemCoordinateX,
                                                                                       itemCoordinateY,
                                                                                       itemCoordinateZ);
                                                        });

        // Prevent Crystal Stars from bosses adjusting the max SP and giving special moves
        patch::hookFunction(ttyd::mario_pouch::pouchGetStarStone, [](uint32_t id) { (void)id; });

        // Get custom messages for specific items/badges
        mPFN_getCustomMsg_trampoline =
            patch::hookFunction(ttyd::msgdrv::msgSearch, [](const char *msgKey) { return gMod->getCustomMsg(msgKey); });

        // Run/set additional things when starting a battle
        mPFN_initBattleStuff_trampoline =
            patch::hookFunction(ttyd::seq_battle::battle_init, []() { return gMod->initBattleStuff(); });

        // LZ Rando
        // Prevent the game from removing partners
        // Prevent partyLeft from running
        mPFN_partyLeft_trampoline =
            patch::hookFunction(ttyd::mario_party::partyLeft, [](ttyd::party::PartyMembers id) { gMod->preventPartyLeft(id); });

        mPFN_randomizeGivenFollower_trampoline = patch::hookFunction(ttyd::party::partyEntry2Pos,
                                                                     [](ttyd::party::PartyMembers id, float x, float y, float z)
                                                                     { return gMod->randomizeGivenFollower(id, x, y, z); });

        // Prevent the escape timer from appearing
        mPFN_countDownStart_trampoline =
            patch::hookFunction(ttyd::countdown::countDownStart,
                                [](uint32_t unk1, uint32_t unk2) { gMod->preventCountDownStart(unk1, unk2); });

        // Prevent the waves in Pirate's Grotto from crashing the game
        mPFN_marioShipForceStop_trampoline =
            patch::hookFunction(ttyd::mot_ship::marioShipForceStop, []() { return gMod->preventMarioShipForceStop(); });

        // Prevent being able to get items while reloading a room
        mPFN_preventGetItemOnReload_trampoline =
            patch::hookFunction(ttyd::mario_pouch::pouchGetItem, [](uint32_t id) { return gMod->preventGetItemOnReload(id); });

        // Prevent the Mario heads from appearing at the end of chapters
        mPFN_preventSpecificFades_trampoline = patch::hookFunction(
            ttyd::fadedrv::fadeEntry,
            [](int type, int duration, uint8_t color[4]) { return gMod->preventSpecificFades(type, duration, color); });

        // Randomize the loading zone
        mPFN_getRandomLZ_trampoline = patch::hookFunction(ttyd::evt_bero::evt_bero_get_info,
                                                          [](void *scriptContext, uint32_t waitMode)
                                                          { return gMod->getRandomLZ(scriptContext, waitMode); });

        // Make changes that are only done once
        Mod::writeOnce();

        // Initialize typesetting early
        ttyd::fontmgr::fontmgrTexSetup();
        patch::hookFunction(ttyd::fontmgr::fontmgrTexSetup, []() {});
    }

    void Mod::updateEarly()
    {
        // Display Stuff
        Mod::displayStuff();

        // Item Rando
        Mod::itemRandoStuff();

        // LZ Rando
        Mod::LZRandoStuff();

        // Call original function
        mPFN_makeKey_trampoline();
    }
} // namespace mod
