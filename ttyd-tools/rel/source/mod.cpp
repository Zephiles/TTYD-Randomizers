#include "mod.h"

#include <ttyd/system.h>
#include <ttyd/fontmgr.h>
#include <ttyd/itemdrv.h>
#include <ttyd/dispdrv.h>
#include <ttyd/string.h>

#include "patch.h"

#include <cstdio>

extern bool LZRando;
extern bool LZRandoChallenge;
extern char *NextMap;
extern char *NextArea;

namespace mod {

Mod *gMod = nullptr;

void main()
{
  Mod *mod = new Mod();
  mod->init();
}

Mod::Mod()
{
  
}

void Mod::init()
{
  gMod = this;
  
  mPFN_makeKey_trampoline = patch::hookFunction(ttyd::system::makeKey, []()
  {
    gMod->updateEarly();
  });
  
  // Show backtrace screen by default
  Mod::backtraceScreen();
  
  // Write dmo_00 to NextMap
  Mod::writeNextMap();
  
  // Item Rando
  mPFN_itemEntry_trampoline = patch::hookFunction(ttyd::itemdrv::itemEntry, [](const char *itemName, uint32_t itemId, uint32_t itemMode, int32_t wasCollectedExpr, void *itemPickupScript, float itemCoordinateX, float itemCoordinateY, float itemCoordinateZ)
  {
    return gMod->getRandomItem(const_cast<char *>(itemName), itemId, itemMode, wasCollectedExpr, reinterpret_cast<uint32_t *>(itemPickupScript), itemCoordinateX, itemCoordinateY, itemCoordinateZ);
  });
  Mod::writeItemRandoAssemblyPatches();
  
  // LZ Rando
  Mod::writeLZRandoAssemblyPatches();
  
  // Initialize typesetting early
  ttyd::fontmgr::fontmgrTexSetup();
  patch::hookFunction(ttyd::fontmgr::fontmgrTexSetup, [](){});
}

void Mod::updateEarly()
{
  // Display Stuff
  if (LZRando)
  {
    ttyd::dispdrv::dispEntry(ttyd::dispdrv::DisplayLayer::kDebug3d, 0, [](ttyd::dispdrv::DisplayLayer layerId, void *user)
    {
      reinterpret_cast<Mod *>(user)->LZRandoStuff();
      if (LZRandoChallenge)
      {
        reinterpret_cast<Mod *>(user)->LZRandoChallengeStuff();
      }
    }, this);
  }
  Mod::changeGameModes();
  ttyd::dispdrv::dispEntry(ttyd::dispdrv::DisplayLayer::k2d, 0, [](ttyd::dispdrv::DisplayLayer layerId, void *user)
  {
    reinterpret_cast<Mod *>(user)->titleScreenStuff();
    reinterpret_cast<Mod *>(user)->gameModes();
  }, this);
  
  // Item Rando
  Mod::manageEnemyHeldItemArray();
  Mod::randomizeShopRewardsSetDoorFlag();
  
  // LZ Rando
  if (LZRando)
  {
    // Only run if the Loading Zone Randomizer is being used
    Mod::setUpNewFile();
    Mod::failsafeCheats();
    Mod::resetValuesOnGameOver();
    Mod::reloadCurrentScreenFlag();
  }
  
  // Additional LZ Rando stuff that needs to run no matter what
  Mod::overwriteNewFileStrings();
  Mod::writeAdditionalLZRandoAssemblyPatches();
  
  // Call original function
  mPFN_makeKey_trampoline();
}

void Mod::backtraceScreen()
{
  #ifdef TTYD_US
    uint32_t DebugModeInitializeAddress = 0x80009B2C;
  #elif defined TTYD_JP
    uint32_t DebugModeInitializeAddress = 0x8000999C;
  #elif defined TTYD_EU
    uint32_t DebugModeInitializeAddress = 0x80009CF0;
  #endif
  
  *reinterpret_cast<uint32_t *>(DebugModeInitializeAddress) = 0x3800FFFF; // li r0,-1
}

void Mod::writeNextMap()
{
  // These are normally not set during the boot process, which can cause issues with string comparisons with other code
  ttyd::string::strcpy(NextMap, "dmo_00");
  ttyd::string::strncpy(NextArea, "dmo_00", 3);
}

}