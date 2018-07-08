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
  void backtraceScreen();
  void writeNextMap();
  
  // Display Stuff
  void LZRandoStuff();
  void LZRandoChallengeStuff();
  void titleScreenStuff();
  void gameModes();
  
  // Item Rando
  void *getRandomItem(const char *itemName, uint32_t itemId, uint32_t itemMode, int32_t wasCollectedExpr, void *itemPickupScript, float itemCoordinateX, float itemCoordinateY, float itemCoordinateZ);
  void manageEnemyHeldItemArray();
  void randomizeShopRewardsSetDoorFlag();
  void writeItemRandoAssemblyPatches();
  void *(*mPFN_itemEntry_trampoline)(const char *name, uint32_t id, uint32_t mode, int32_t wasCollectedExpr, void *pickupScript, float coordinateX, float coordinateY, float coordinateZ) = nullptr;
  
  // LZ Rando
  void setUpNewFile();
  void overwriteNewFileStrings();
  void failsafeCheats();
  void resetValuesOnGameOver();
  void reloadCurrentScreenFlag();
  void writeLZRandoAssemblyPatches();
  void writeAdditionalLZRandoAssemblyPatches();
  
private:
  void (*mPFN_makeKey_trampoline)() = nullptr;
  char mDisplayBuffer[256];
};

}