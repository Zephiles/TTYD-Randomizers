#include "mod.h"
#include "maps.h"
#include "buttons.h"
#include "patch.h"

#include <ttyd/string.h>
#include <ttyd/swdrv.h>
#include <ttyd/system.h>
#include <ttyd/mario_party.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/seqdrv.h>
#include <ttyd/mario.h>
#include <ttyd/mariost.h>
#include <ttyd/camdrv.h>
#include <ttyd/pmario_sound.h>
#include <ttyd/mario_cam.h>
#include <ttyd/OSCache.h>

#include <cstdio>

extern bool ReloadCurrentScreen;
extern bool GameOverFlag;
extern char *NextMap;
extern bool NewFile;
extern uint32_t GSWAddressesStart;
extern uint32_t PossibleMaps[];
extern uint16_t MapArraySize;
extern char *NextArea;
extern char *NextBero;
extern char *NewBero;
extern bool LZRando;
extern bool ClearCacheNewFileStrings;
extern char *NewMap;
extern uint32_t seqMainAddress;
extern bool ClearCacheFlag;

extern "C" {
  void StartGetRandomWarp();
  void BranchBackGetRandomWarp();
  void StartEnablePaperTubeModes();
  void StartEnableBoatMode();
  void BranchBackEnableBoatMode();
}

namespace mod {

extern "C" {
void getRandomWarp()
{
  // This function is ran from a place that only runs once during a screen transition
  // Don't run if currently reloading the current screen
  if (!ReloadCurrentScreen && (GameOverFlag || ((ttyd::string::strcmp(NextMap, "dmo_00") != 0) && (ttyd::string::strcmp(NextMap, "title") != 0))))
  {
    // Don't run if transitioning to the intro or the title screen, unless the Game Over flag is set
    GameOverFlag = false;
    
    // Prevent random warp upon starting a new file
    uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
    if (SequencePosition == 0)
    {
      // Set flag to allow new file stuff to be set up
      NewFile = true;
    }
    else
    {
      uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
      bool ConfirmNewMap = false;
      
      while (!ConfirmNewMap)
      {
        // Get new map to warp to
        char *NewRandomMap = reinterpret_cast<char *>(PossibleMaps[ttyd::system::irand(MapArraySize)]);
        ttyd::string::strcpy(NextMap, NewRandomMap);
        ttyd::string::strncpy(NextArea, NewRandomMap, 3);
        
        if (ttyd::string::strcmp(NextMap, "eki_03") == 0)
        {
          // Change loading zone to avoid softlocking
          ttyd::string::strcpy(NextBero, "w_bero_1");
        }
        else if (ttyd::string::strcmp(NextMap, "eki_05") == 0)
        {
          // Change the loading zone used if the player has not opened the Ultra Boots chest yet
          uint32_t UltraBootsChest = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x348); // GSWF(3728)
          
          if (!(UltraBootsChest & (1 << 16))) // Check if the 16 bit is on or off
          {
            // Run if the 16 bit is off
            ttyd::string::strcpy(NextBero, "w_bero_1");
          }
        }
        else if (ttyd::string::strcmp(NextMap, "end_00") == 0)
        {
          #ifdef TTYD_US
            uint16_t StarValue = 0xDE00;
          #elif defined TTYD_JP
            uint16_t StarValue = 0x8199;
          #elif defined TTYD_EU
            uint16_t StarValue = 0xDE00;
          #endif
          
          uint16_t CurrentNameChars = *reinterpret_cast<uint16_t *>(GSWAddresses + 0x11BC);
          
          // Don't warp to credits if using the Star character as the file name
          if (CurrentNameChars == StarValue)
          {
            continue;
          }
        }
        else if (ttyd::string::strcmp(NextMap, "mri_16") == 0)
        {
          // Change loading zone to avoid softlocking
          ttyd::string::strcpy(NextBero, "dokan1");
        }
        else if (ttyd::string::strcmp(NextMap, "rsh_05_a") == 0)
        {
          // The game will crash if the player enters this room with the sequence being greater than 338
          if (SequencePosition > 338)
          {
            // Get a new map
            continue;
          }
        }
        else if (ttyd::string::strcmp(NextMap, "jon_00") == 0)
        {
          // Adjust Pit floor if a Pit room is selected
          uint8_t NewPitFloor = ttyd::system::irand(100);
          uint32_t NewPitMap;
          
          // Get proper room to use for chosen floor
          if (NewPitFloor % 10 == 9)
          {
            // Current floor is a chest floor or Bonetail
            if (NewPitFloor < 40)
            {
              NewPitMap = jon_03;
            }
            else if (NewPitFloor < 70)
            {
              NewPitMap = jon_04;
            }
            else if (NewPitFloor < 90)
            {
              NewPitMap = jon_05;
            }
            else // NewPitFloor == 99
            {
              NewPitMap = jon_06;
            }
          }
          else // Current floor is not a chest floor nor Bonetail
          {
            if (NewPitFloor < 50)
            {
              NewPitMap = jon_00;
            }
            else if (NewPitFloor < 80)
            {
              NewPitMap = jon_01;
            }
            else // NewPitMap < 100
            {
              NewPitMap = jon_02;
            }
          }
          
          // Set Pit floor
          *reinterpret_cast<uint8_t *>(GSWAddresses + 0xAA1) = NewPitFloor; // GSW(1321)
          
          // Reset NextMap to proper Pit map
          ttyd::string::strcpy(NextMap, reinterpret_cast<char *>(NewPitMap));
          
          // Change loading zone to the pipe above the room
          ttyd::string::strcpy(NextBero, "dokan_2");
        }
        
        ConfirmNewMap = true;
      }
    }
  }
}
}

void Mod::setUpNewFile()
{
  // Set up stuff for new file
  if ((ttyd::swdrv::swByteGet(0) == 0) && NewFile)
  {
    NewFile = false;
    
    // Set Sequence to be just past getting Goombella
    ttyd::swdrv::swByteSet(0, 7);
    
    // Turn off a bit to enable loading zones
    uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
    *reinterpret_cast<uint32_t *>(GSWAddresses) &= ~(1 << 0); // Turn off the 0 bit
    
    // Turn on GSWF(0) to skip shop tutorials
    *reinterpret_cast<uint32_t *>(GSWAddresses + 0x178) |= (1 << 0); // Turn on the 0 bit
    
    // Turn on GSWF(37) and GSWF(38) to prevent emails from being explained for the fitst time
    *reinterpret_cast<uint32_t *>(GSWAddresses + 0x17C) |= ((1 << 5) | (1 << 6)); // Turn on the 5 and 6 bits
    
    // Turn on GSWF(233) to skip save blocks from being explained for the first time
    *reinterpret_cast<uint32_t *>(GSWAddresses + 0x194) |= (1 << 9); // Turn on the 9 bit
    
    // Turn on GSWF(234) to skip recovery blocks from being explained for the first time
    *reinterpret_cast<uint32_t *>(GSWAddresses + 0x194) |= (1 << 10); // Turn on the 10 bit
    
    // Turn on GSWF(235) to skip items from being explained for the first time
    *reinterpret_cast<uint32_t *>(GSWAddresses + 0x194) |= (1 << 11); // Turn on the 11 bit
    
    // Turn on GSWF(1200) to prevent partners from explaining Save Blocks in central Rogueport
    *reinterpret_cast<uint32_t *>(GSWAddresses + 0x20C) |= (1 << 16); // Turn on the 16 bit
    
    // Add all partners
    for (int i = 1; i <= 7; i++)
    {
      ttyd::mario_party::partyJoin(i);
    }
    
    // Randomize Yoshi color
    ttyd::mario_pouch::pouchSetPartyColor(4, ttyd::system::irand(7));
    
    // Start with Yoshi out
    ttyd::mario_party::marioPartyHello(4);
  }
}

void Mod::overwriteNewFileStrings()
{
  // Overwrite aaa_00 and prologue strings, and put back to default when new values are not needed anymore
  #ifdef TTYD_US
    uint32_t aaa_00_Address = 0x802EDE78;
  #elif defined TTYD_JP
    uint32_t aaa_00_Address = 0x802ED930;
  #elif defined TTYD_EU
    uint32_t aaa_00_Address = 0x802F9AD8;
  #endif
  uint32_t prologue_Address = aaa_00_Address + 0x8;
  
  uint32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  
  if ((NextSeq == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kLoad)) && (seqMainCheck == 2))
  {
    // Set the cache of the strings to be cleared later
    ClearCacheNewFileStrings = false;
    
    // Set strings on file select screen curtain
    if (LZRando)
    {
      ttyd::string::strcpy(reinterpret_cast<char *>(aaa_00_Address), "gor_01");
      ttyd::string::strcpy(reinterpret_cast<char *>(prologue_Address), "s_bero");
    }
  }
  else if (!LZRando || ((NextSeq == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame)) && (ttyd::swdrv::swByteGet(0) > 0)))
  {
    // Set strings to default when not using the Loading Zone randomizer, or when the file is already loaded and in central Rogueport
    ttyd::string::strcpy(reinterpret_cast<char *>(aaa_00_Address), "aaa_00");
    ttyd::string::strcpy(reinterpret_cast<char *>(prologue_Address), "prologue");
  }
  
  // Clear the cache of the strings upon starting a file
  if (!ClearCacheNewFileStrings && (NextSeq == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame)))
  {
    ClearCacheNewFileStrings = true;
    ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(aaa_00_Address), 16);
    ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(aaa_00_Address), 16);
  }
}

void marioNeverTransform()
{
  if (ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame))
  {
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
  
    // Set characterId to Mario if not already set
    if (player->characterId != 0)
    {
      ttyd::mario::marioSetCharMode(0);
    }
  }
}

void dismountYoshi()
{
  // This needs to be possible in the event that a cutscene forces Mario off of Yoshi improperly
  uint16_t DismountYoshiCombo = PAD_L | PAD_Z;
  if (((ttyd::system::keyGetButton(0) & DismountYoshiCombo) == DismountYoshiCombo) && (ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame)))
  {
    uint16_t CurrentlyUsingYoshi = 26;
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
    if (player->currentMotionId == CurrentlyUsingYoshi)
    {
      // Dismount Yoshi, as well as manually clearing some stuff to allow the player to move around again
      player->currentMotionId = 0;
      player->flags2 &= ~((1 << 12) | (1 << 2)); // Turn off the 12 and 2 bits
      player->wAnimPosition[1] = 0;
      player->wModelPosition[0] = 0;
      player->wModelPosition[2] = 0;
    }
  }
}

void resetMarioThroughLZ()
{
  // Prevent softlocks by resetting Mario back to normal upon entering a loading zone
  if (ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kMapChange))
  {
    uint16_t CurrentlyUsingTubeMode = 22;
    uint16_t CurrentlyUsingBoatMode = 25;
    uint16_t CurrentlyUsingYoshi = 26;
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
    if ((player->currentMotionId == CurrentlyUsingTubeMode) || (player->currentMotionId == CurrentlyUsingBoatMode) || (player->currentMotionId == CurrentlyUsingYoshi))
    {
      player->currentMotionId = 0;
    }
  }
}

void reloadScreen()
{
  uint16_t ReloadScreenCombo = PAD_L | PAD_B;
  if ((ttyd::system::keyGetButton(0) & ReloadScreenCombo) == (ReloadScreenCombo))
  {
    uint32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
    uint32_t Game = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame);
    uint32_t MapChange = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kMapChange);
    
    if ((NextSeq >= Game) && (NextSeq <= MapChange))
    {
      // Not in the pause menu
      // A separate address for NextBero is needed, as the original value will be cleared during the reloading process
      // The game will crash if NextMap is used directly in seqSetSeq, so a separate address must be used instead
      
      ttyd::string::strcpy(NewBero, NextBero);
      ttyd::string::strcpy(NewMap, NextMap);
      ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, NewMap, NewBero);
      ReloadCurrentScreen = true;
      
      uint32_t SystemLevel = ttyd::mariost::marioStGetSystemLevel();
      if (SystemLevel != 0)
      {
        if (SystemLevel == 15)
        {
          // Currently in pause menu, so re-enable the camera
          ttyd::camdrv::camDispOn(4);
        }
        
        // Enable sound effects, set the default camera id for Mario, and give back control to the player
        ttyd::pmario_sound::psndClearFlag(0x80);
        ttyd::mario_cam::marioSetCamId(4);
        ttyd::mariost::marioStSystemLevel(0);
      }
    }
  }
}

void setNextBero()
{
  // Prevent changing the loading zone if currently in a room where using it would result in a softlock
  if ((ttyd::string::strcmp(NextMap, "mri_16") != 0) && (ttyd::string::strcmp(NextMap, "eki_03") != 0))
  {
    uint16_t NBeroCombo = PAD_L | PAD_DPAD_UP;
    uint16_t SBeroCombo = PAD_L | PAD_DPAD_DOWN;
    uint16_t WBeroCombo = PAD_L | PAD_DPAD_LEFT;
    uint16_t EBeroCombo = PAD_L | PAD_DPAD_RIGHT;
    
    uint32_t ButtonInput = ttyd::system::keyGetButton(0);
    if ((ButtonInput & NBeroCombo) == (NBeroCombo))
    {
      ttyd::string::strcpy(NextBero, "n_bero");
    }
    else if ((ButtonInput & SBeroCombo) == (SBeroCombo))
    {
      ttyd::string::strcpy(NextBero, "s_bero");
    }
    else if ((ButtonInput & WBeroCombo) == (WBeroCombo))
    {
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if ((ButtonInput & EBeroCombo) == (EBeroCombo))
    {
      ttyd::string::strcpy(NextBero, "e_bero");
    }
  }
}

void Mod::failsafeCheats()
{
  marioNeverTransform();
  resetMarioThroughLZ();
  dismountYoshi();
  reloadScreen();
  setNextBero();
}

void Mod::resetValuesOnGameOver()
{
  if (ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGameOver))
  {
    NewFile = false;
    ReloadCurrentScreen = false;
    GameOverFlag = true;
  }
}

void Mod::reloadCurrentScreenFlag()
{
  if (ReloadCurrentScreen && (ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame)))
  {
    ReloadCurrentScreen = false;
  }
}

void Mod::writeLZRandoAssemblyPatches()
{
  #ifdef TTYD_US
    uint32_t RandomWarp = 0x800087C8;
    uint32_t TubeModeCheck = 0x80092344;
    uint32_t PaperModeCheck = 0x800A6A3C;
    uint32_t BoatModeCheck = 0x8009268C;
  #elif defined TTYD_JP
    uint32_t RandomWarp = 0x800086F0;
    uint32_t TubeModeCheck = 0x80090D90;
    uint32_t PaperModeCheck = 0x800A4DB4;
    uint32_t BoatModeCheck = 0x800910D8;
  #elif defined TTYD_EU
    uint32_t RandomWarp = 0x80008994;
    uint32_t TubeModeCheck = 0x800936A0;
    uint32_t PaperModeCheck = 0x800A7E0C;
    uint32_t BoatModeCheck = 0x800939E8;
  #endif
  
  // Random Warps
  patch::writeBranch(reinterpret_cast<void *>(RandomWarp), reinterpret_cast<void *>(StartGetRandomWarp));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackGetRandomWarp), reinterpret_cast<void *>(RandomWarp + 0x4));
  
  // Allow Paper and Tube modes to be used in the Creepy Steeple room with the Cook Book
  patch::writeBranch(reinterpret_cast<void *>(TubeModeCheck), reinterpret_cast<void *>(StartEnablePaperTubeModes));
  patch::writeBranch(reinterpret_cast<void *>(PaperModeCheck), reinterpret_cast<void *>(StartEnablePaperTubeModes));
  
  // Adjust branches to be bl instead of b; should modify the patch function to allow for this instead
  *reinterpret_cast<uint32_t *>(TubeModeCheck) = *reinterpret_cast<uint32_t *>(TubeModeCheck) + 0x1;
  *reinterpret_cast<uint32_t *>(PaperModeCheck) = *reinterpret_cast<uint32_t *>(PaperModeCheck) + 0x1;
  
  // Allow Boat mode to be used in the Rogueport Sewers room with the 3 Shine Sprites
  patch::writeBranch(reinterpret_cast<void *>(BoatModeCheck), reinterpret_cast<void *>(StartEnableBoatMode));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackEnableBoatMode), reinterpret_cast<void *>(BoatModeCheck + 0x4));
}

void Mod::writeAdditionalLZRandoAssemblyPatches()
{
  #ifdef TTYD_US
    uint32_t WalkOnWater1 = 0x8008F938;
    uint32_t WalkOnWater2 = 0x80092DF8;
    uint32_t aaa_00_Address = 0x802EDE78;
  #elif defined TTYD_JP
    uint32_t WalkOnWater1 = 0x8008E384;
    uint32_t WalkOnWater2 = 0x80091844;
    uint32_t aaa_00_Address = 0x802ED930;
  #elif defined TTYD_EU
    uint32_t WalkOnWater1 = 0x80090C94;
    uint32_t WalkOnWater2 = 0x80094174;
    uint32_t WalkOnWater3 = 0x80093CEC;
    uint32_t aaa_00_Address = 0x802F9AD8;
  #endif
  
  if (!LZRando)
  {
    *reinterpret_cast<uint32_t *>(WalkOnWater1) = 0x4182003C; // beq- 0x3C
    *reinterpret_cast<uint32_t *>(WalkOnWater2) = 0x418200A4; // beq- 0xA4
    #ifdef TTYD_EU
      // Allow Mario to jump while standing on water
      *reinterpret_cast<uint32_t *>(WalkOnWater3) = 0x807F01E8; // lwz r3,0x01E8(r31)
    #endif
  }
  else
  {
    // Allow Mario to walk on water to avoid softlocks
    *reinterpret_cast<uint32_t *>(WalkOnWater1) = 0x4800003C; // b 0x3C
    *reinterpret_cast<uint32_t *>(WalkOnWater2) = 0x480000A4; // b 0xA4
    #ifdef TTYD_EU
      // Allow Mario to jump while standing on water
      *reinterpret_cast<uint32_t *>(WalkOnWater3) = 0x38600000; // li r3,0
    #endif
  }
  
  // Clear Cache
  uint32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  if (!ClearCacheFlag && (NextSeq == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kLoad)) && (seqMainCheck == 4))
  {
    ClearCacheFlag = true;
    
    ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(WalkOnWater1), 4);
    ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(WalkOnWater1), 4);
    
    ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(WalkOnWater2), 4);
    ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(WalkOnWater2), 4);
    
    #ifdef TTYD_EU
      ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(WalkOnWater3), 4);
      ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(WalkOnWater3), 4);
    #endif
    
    ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(aaa_00_Address), 16);
    ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(aaa_00_Address), 16);
  }
  else if (ClearCacheFlag && ((NextSeq == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame)) || (NextSeq == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kTitle))))
  {
    ClearCacheFlag = false;
  }
}

}