#pragma once

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
  
private:
  void (*mPFN_makeKey_trampoline)() = nullptr;
};

}