#pragma once

#include <ttyd/party.h>

#include <cstdint>

namespace mod {

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
  uint32_t warpAwayFromSQ(void *);
  uint32_t (*mPFN_warpAwayFromSQ_trampoline)(void *) = nullptr;
  bool preventGetItemOnReload(uint32_t);
  bool (*mPFN_preventGetItemOnReload_trampoline)(uint32_t) = nullptr;
  void preventMarioEndOfChapterHeads(int, int, uint8_t *);
  void (*mPFN_preventMarioEndOfChapterHeads_trampoline)(int, int, uint8_t *) = nullptr;
  
private:
  void (*mPFN_makeKey_trampoline)() = nullptr;
};

}