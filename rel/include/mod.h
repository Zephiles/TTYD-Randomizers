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
  void preventPartyLeft(ttyd::party::Party);
  void (*mPFN_partyLeft_trampoline)(ttyd::party::Party) = nullptr;
  int32_t randomizeGivenFollower(ttyd::party::Party, float, float, float);
  int32_t (*mPFN_randomizeGivenFollower_trampoline)(ttyd::party::Party, float, float, float) = nullptr;
  void preventCountDownStart(uint32_t, uint32_t);
  void (*mPFN_countDownStart_trampoline)(uint32_t, uint32_t) = nullptr;
  void *preventMarioShipForceStop();
  void *(*mPFN_marioShipForceStop_trampoline)() = nullptr;
  
private:
  void (*mPFN_makeKey_trampoline)() = nullptr;
};

}