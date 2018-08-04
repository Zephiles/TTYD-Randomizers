#include "mod.h"

#include <ttyd/system.h>
#include <ttyd/itemdrv.h>
#include <ttyd/fontmgr.h>
#include <ttyd/mario_party.h>
#include <ttyd/countdown.h>
#include <ttyd/mot_ship.h>

#include "patch.h"

#include <cstdio>

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
  
  // Item Rando
  mPFN_itemEntry_trampoline = patch::hookFunction(ttyd::itemdrv::itemEntry, [](const char *itemName, uint32_t itemId, uint32_t itemMode, int32_t wasCollectedExpr, void *itemPickupScript, float itemCoordinateX, float itemCoordinateY, float itemCoordinateZ)
  {
    return gMod->getRandomItem(const_cast<char *>(itemName), itemId, itemMode, wasCollectedExpr, reinterpret_cast<uint32_t *>(itemPickupScript), itemCoordinateX, itemCoordinateY, itemCoordinateZ);
  });
  
  // LZ Rando
  // Prevent the game from removing partners
  // Prevent partyLeft from running
  mPFN_partyLeft_trampoline = patch::hookFunction(ttyd::mario_party::partyLeft, [](uint32_t id)
  {
    gMod->preventPartyLeft(id);
  });
  
  // Prevent the escape timer from appearing
  mPFN_countDownStart_trampoline = patch::hookFunction(ttyd::countdown::countDownStart, [](uint32_t unk1, uint32_t unk2)
  {
    gMod->preventCountDownStart(unk1, unk2);
  });
  
  // Prevent the waves in Pirate's Grotto from crashing the game
  mPFN_marioShipForceStop_trampoline = patch::hookFunction(ttyd::mot_ship::marioShipForceStop, []()
  {
    return gMod->preventMarioShipForceStop();
  });
  
  // Make changes that are only done once
  Mod::writeOnce();
  
  // Initialize typesetting early
  ttyd::fontmgr::fontmgrTexSetup();
  patch::hookFunction(ttyd::fontmgr::fontmgrTexSetup, [](){});
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

}