#pragma once

#include <ttyd/party.h>

#include <cstdint>

namespace mod
{
    class Mod
    {
       public:
        Mod();
        void init();

       private:
        void updateEarly();
        void writeOnce();

        // Display Stuff
        void displayStuff();
        void helpMenu();
        void LZRandoDisplayStuff();
        void LZRandoChallengeStuff();
        void titleScreenStuff();
        void gameModes();

        // Item Rando
        void itemRandoStuff();
        void writeItemRandoAssemblyPatches();
        void *getRandomItem(const char *, uint32_t, uint32_t, int32_t, void *, float, float, float);
        void *(*mPFN_itemEntry_trampoline)(const char *, uint32_t, uint32_t, int32_t, void *, float, float, float) = nullptr;
        const char *getCustomMsg(const char *);
        const char *(*mPFN_getCustomMsg_trampoline)(const char *) = nullptr;
        bool initBattleStuff();
        bool (*mPFN_initBattleStuff_trampoline)() = nullptr;

        // LZ Rando
        void LZRandoStuff();
        void writeLZRandoAssemblyPatches();
        void preventPartyLeft(ttyd::party::PartyMembers);
        void (*mPFN_partyLeft_trampoline)(ttyd::party::PartyMembers) = nullptr;
        int32_t randomizeGivenFollower(ttyd::party::PartyMembers, float, float, float);
        int32_t (*mPFN_randomizeGivenFollower_trampoline)(ttyd::party::PartyMembers, float, float, float) = nullptr;
        void preventCountDownStart(uint32_t, uint32_t);
        void (*mPFN_countDownStart_trampoline)(uint32_t, uint32_t) = nullptr;
        void *preventMarioShipForceStop();
        void *(*mPFN_marioShipForceStop_trampoline)() = nullptr;
        bool preventGetItemOnReload(uint32_t);
        bool (*mPFN_preventGetItemOnReload_trampoline)(uint32_t) = nullptr;
        void preventSpecificFades(int, int, uint8_t[4]);
        void (*mPFN_preventSpecificFades_trampoline)(int, int, uint8_t[4]) = nullptr;
        uint32_t getRandomLZ(void *, uint32_t);
        uint32_t (*mPFN_getRandomLZ_trampoline)(void *, uint32_t) = nullptr;

       private:
        void (*mPFN_makeKey_trampoline)() = nullptr;
    };
} // namespace mod
