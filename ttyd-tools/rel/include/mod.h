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
  void *getRandomItem(const char *itemName, uint32_t itemId, uint32_t itemMode, int32_t wasCollectedExpr, void *itemPickupScript, float itemCoordinateX, float itemCoordinateY, float itemCoordinateZ);
  void *(*mPFN_itemEntry_trampoline)(const char *name, uint32_t id, uint32_t mode, int32_t wasCollectedExpr, void *pickupScript, float coordinateX, float coordinateY, float coordinateZ) = nullptr;
  
  // LZ Rando
  void LZRandoStuff();
  void writeLZRandoAssemblyPatches();
  
private:
  void (*mPFN_makeKey_trampoline)() = nullptr;
};

}