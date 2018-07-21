#include "mod.h"
#include "maps.h"
#include "items.h"
#include "buttons.h"
#include "patch.h"

#include <ttyd/string.h>
#include <ttyd/swdrv.h>
#include <ttyd/system.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/mario_party.h>
#include <ttyd/party.h>
#include <ttyd/seqdrv.h>
#include <ttyd/OSCache.h>
#include <ttyd/mario.h>
#include <ttyd/mariost.h>
#include <ttyd/camdrv.h>
#include <ttyd/pmario_sound.h>
#include <ttyd/mario_cam.h>

#include <cstdio>

extern bool LZRando;
extern bool ReloadCurrentScreen;
extern char *NextMap;
extern bool InCredits;
extern bool GameOverFlag;
extern char *NextBero;
extern char *NextArea;
extern bool NewFile;
extern uint32_t GSWAddressesStart;
extern uint32_t PossibleMaps[];
extern uint16_t MapArraySize;
extern bool LZRandoChallenge;
extern uint32_t TimerCount;
extern uint32_t seqMainAddress;
extern bool ClearCacheNewFileStrings;
// extern uint32_t AreaObjectsAddressesStart;
extern uint32_t AreaLZsAddressesStart;
// extern uint32_t NPCAddressesStart;
extern char *NewBero;
extern char *NewMap;
extern bool ClearCacheFlag;

extern "C" {
  void StartGetRandomWarp();
  void BranchBackGetRandomWarp();
  void StartEnablePaperTubeModes();
  void StartEnableBoatMode();
  void BranchBackEnableBoatMode();
  void StartMarioKeyOnFix();
  void BranchBackMarioKeyOnFix();
}

namespace mod {

extern "C" {
void getRandomWarp()
{
  // This function is ran from a place that only runs once during a screen transition
  // Don't run if the Loading Zone randomizer is currently not in use
  if (!LZRando)
  {
    return;
  }
  
  // Don't run if currently reloading the current screen
  if (ReloadCurrentScreen)
  {
    return;
  }
  
  bool title_comparison = ttyd::string::strcmp(NextMap, "title") == 0;
  
  // Don't run on the title screen, unless the credits just occured
  if (title_comparison && !InCredits)
  {
    return;
  }
  
  // Don't run if transitioning to the intro
  // However, it should still run under these conditions if the Game Over flag is set
  if (ttyd::string::strcmp(NextMap, "dmo_00") == 0)
  {
    if (!GameOverFlag)
    {
      return;
    }
  }
  
  // Warp back to Magnus 2.0's room if the player just beat him
  if (ttyd::string::strcmp(NextBero, "evt_aji_14") == 0)
  {
    ttyd::string::strcpy(NextBero, "tuzuki");
    ttyd::string::strcpy(NextMap, "aji_14");
    ttyd::string::strncpy(NextArea, "aji_14", 3);
    return;
  }
  
  // Warp back to the Shadow Queen room if currently in that Sequence
  if (ttyd::string::strcmp(NextBero, "minnnanokoe") == 0)
  {
    ttyd::string::strcpy(NextMap, "las_29");
    ttyd::string::strncpy(NextArea, "las_29", 3);
    return;
  }
  
  // Don't run on the credits screen
  if (ttyd::string::strcmp(NextMap, "end_00") == 0)
  {
    return;
  }
  
  // Prevent random warp upon starting a new file
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  if (SequencePosition == 0)
  {
    // Set flag to allow new file stuff to be set up
    NewFile = true;
    return;
  }
  
  GameOverFlag = false;
  uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
  uint32_t TimerCountCutoff = 144000; // 40 Minutes
  bool ConfirmNewMap = false;
  
  while (!ConfirmNewMap)
  {
    // Get new map to warp to
    char *NewRandomMap = reinterpret_cast<char *>(PossibleMaps[ttyd::system::irand(MapArraySize)]);
    ttyd::string::strcpy(NextMap, NewRandomMap);
    ttyd::string::strncpy(NextArea, NewRandomMap, 3);
    
    if (ttyd::string::strcmp(NextMap, "aji_14") == 0)
    {
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 373
      if ((SequencePosition < 373) && LZRandoChallenge && (TimerCount >= TimerCountCutoff))
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "eki_03") == 0)
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
      // Prevent warping to the credits if not using the challenge mode
      if (!LZRandoChallenge)
      {
        continue;
      }
      
      #ifdef TTYD_US
        uint8_t StarValue = 0xDE;
      #elif defined TTYD_JP
        uint16_t StarValue = 0x8199;
      #elif defined TTYD_EU
        uint8_t StarValue = 0xDE;
      #endif
      
      #ifdef TTYD_JP
        // Get the first 2 bytes of the file name
        uint16_t CurrentNameChars = *reinterpret_cast<uint16_t *>(GSWAddresses + 0x11BC);
      #else
        // Get the first byte of the file name
        uint8_t CurrentNameChars = *reinterpret_cast<uint8_t *>(GSWAddresses + 0x11BC);
      #endif
      
      // Don't warp to credits if using the Star character as the file name
      if (CurrentNameChars == StarValue)
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_11") == 0)
    {
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 55
      if ((SequencePosition < 55) && LZRandoChallenge && (TimerCount >= TimerCountCutoff))
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_12") == 0)
    {
      // Skip the Mowz cutscene
      if (SequencePosition < 50)
      {
        // Set the Sequence to 50 to prevent the cutscene from playing
        ttyd::swdrv::swByteSet(0, 50);
      }
    }
    else if (ttyd::string::strcmp(NextMap, "jin_00") == 0)
    {
      // Check if the player has either of the Hammer upgrades
      bool SuperHammerAmount = ttyd::mario_pouch::pouchCheckItem(SuperHammer) > 0;
      bool UltraHammerAmount = ttyd::mario_pouch::pouchCheckItem(UltraHammer) > 0;
      
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the player has a Hammer upgrade
      if ((SuperHammerAmount || UltraHammerAmount) && LZRandoChallenge && (TimerCount >= TimerCountCutoff))
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "jin_04") == 0)
    {
      // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
      if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
      {
        continue;
      }
      
      // Allow Doopliss 1 or 2 to be fought at random
      int32_t DooplissCheck = ttyd::system::irand(2);
      
      if (DooplissCheck == 0)
      {
        if (SequencePosition < 199)
        {
          // Set up the Sequence to fight Doopliss 1
          ttyd::swdrv::swByteSet(0, 199);
        }
      }
      else // DooplissCheck == 1
      {
        if (SequencePosition < 210)
        {
          // Set up the Sequence to fight Doopliss 2
          ttyd::swdrv::swByteSet(0, 210);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "jon_00") == 0)
    {
      // Adjust Pit floor if a Pit room is selected
      uint32_t TotalPitFloors = 100;
      
      if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
      {
        // Prevent Bonetail from being chosen if currently using the challenge mode and 20 minutes have not passed yet
        TotalPitFloors--;
      }
      
      uint8_t NewPitFloor = ttyd::system::irand(TotalPitFloors);
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
    else if (ttyd::string::strcmp(NextMap, "las_09") == 0)
    {
      if (SequencePosition < 390)
      {
        if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow the Shadow Sirens to be fought if the Sequence is not past 390
          // Set the Sequence to 390 so that the Shadow Sirens can be fought
          ttyd::swdrv::swByteSet(0, 390);
          ttyd::string::strcpy(NextBero, "e_bero");
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_26") == 0)
    {
      if (SequencePosition < 387)
      {
        if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Gloomtail to be fought if the Sequence is not past 387
          // Set the Sequence to 387 so that Gloomtail can be fought
          ttyd::swdrv::swByteSet(0, 387);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_29") == 0)
    {
      if (SequencePosition < 400)
      {
        if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow the Shadow Queen to be fought if the Sequence is not past 400
          // Set the Sequence to 400 so that the Shadow Queen can be fought
          ttyd::swdrv::swByteSet(0, 400);
          ttyd::string::strcpy(NextBero, "sekai_yami2");
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "mri_01") == 0)
    {
      if (SequencePosition < 110)
      {
        if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Magnus 1.0 to be fought if the Sequence is not past 110
          // Set the Sequence to 110 so that Magnus 1.0 can be fought
          ttyd::swdrv::swByteSet(0, 110);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "muj_12") == 0)
    {
      if (SequencePosition < 252)
      {
        if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Cortez to be fought if the Sequence is not past 252
          // Set the Sequence to 252 so that Cortez can be fought
          ttyd::swdrv::swByteSet(0, 252);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "rsh_05_a") == 0)
    {
      // The game will crash if the player enters this room with the Sequence being greater than 338
      if (SequencePosition > 338)
      {
        // Get a new map
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "rsh_06_a") == 0)
    {
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 332
      if ((SequencePosition < 332) && LZRandoChallenge && (TimerCount >= TimerCountCutoff))
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tik_02") == 0)
    {
      if (SequencePosition < 20)
      {
        if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Blooper to be fought if the Sequence is not past 20
          // Set the Sequence to 20 so that Blooper can be fought
          ttyd::swdrv::swByteSet(0, 20);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tou_03") == 0)
    {
      if (SequencePosition < 163)
      {
        if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Grubba to be fought if the Sequence is not past 163
          // Set the Sequence to 163 so that Grubba can be fought
          ttyd::swdrv::swByteSet(0, 163);
        }
      }
    }
    
    ConfirmNewMap = true;
  }
}
}

void setUpNewFile()
{
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  bool Comparisons = (SequencePosition == 0) && NewFile;
  
  if (!Comparisons)
  {
    return;
  }
  
  NewFile = false;
  
  // Set up stuff for new file
  // Set Sequence to be just past getting Goombella
  ttyd::swdrv::swByteSet(0, 7);
  
  // Add all partners
  for (int i = 1; i <= 7; i++)
  {
    ttyd::mario_party::partyJoin(i);
  }
  
  // Randomize Yoshi color
  ttyd::party::yoshiSetColor();
  
  // Start with Yoshi out
  ttyd::mario_party::marioPartyHello(4);
  
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
  
  // Turn on GSWF(1781) and GSWF(1782) to skip the Koopie Koo cutscene in Petal Meadows
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x254) |= ((1 << 21) | (1 << 22)); // Turn on the 21 and 22 bits
  
  // Turn on GSWF(2867) to drain the water in the Great Tree
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2DC) |= (1 << 19); // Turn on the 19 bit
  
  // Turn on GSWF(2878) to prevent the player from being able to talk to Jabble
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2DC) |= (1 << 30); // Turn on the 30 bit
  
  // Turn on GSWF(2982), GSWF(2983), and GSWF(2984) to activate the blue switches in Pirate's Grotto
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2EC) |= ((1 << 6) | (1 << 7) | (1 << 8)); // Turn on the 6, 7, and 8 bits
}

void overwriteNewFileStrings()
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
  
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Load = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kLoad);
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  
  if ((NextSeq == Load) && (seqMainCheck == 2))
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
  else if (!LZRando || ((NextSeq == Game) && (SequencePosition > 0)))
  {
    // Set strings to default when not using the Loading Zone randomizer, or when the file is already loaded and in central Rogueport
    ttyd::string::strcpy(reinterpret_cast<char *>(aaa_00_Address), "aaa_00");
    ttyd::string::strcpy(reinterpret_cast<char *>(prologue_Address), "prologue");
  }
  
  // Clear the cache of the strings upon starting a file
  if (!ClearCacheNewFileStrings && (NextSeq == Game))
  {
    ClearCacheNewFileStrings = true;
    ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(aaa_00_Address), 16);
    ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(aaa_00_Address), 16);
  }
}

void marioNeverTransform()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  
  if (NextSeq != Game)
  {
    return;
  }
  
  ttyd::mario::Player *player = ttyd::mario::marioGetPtr();

  // Set characterId to Mario if not already set
  if (player->characterId != 0)
  {
    ttyd::mario::marioSetCharMode(0);
  }
}

void specificMapEdits()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  
  if (NextSeq != Game)
  {
    return;
  }
  
  uint32_t PartnerPointer = reinterpret_cast<uint32_t>(ttyd::party::partyGetPtr(ttyd::mario_party::marioGetPartyId()));
  // uint32_t AreaObjectAddresses = *reinterpret_cast<uint32_t *>(AreaObjectsAddressesStart + 0x4);
  uint32_t AreaLZAddresses = *reinterpret_cast<uint32_t *>(AreaLZsAddressesStart);
  AreaLZAddresses = *reinterpret_cast<uint32_t *>(AreaLZAddresses + 0x4);
  
  if (ttyd::string::strcmp(NextMap, "jon_06") == 0)
  {
    // Make sure a partner is out when fighting Bonetail
    // This can be avoided with the edits to the jon REL, which will probably be implemented at a later date
    if (!PartnerPointer)
    {
      // Bring out Yoshi if no partner is out
      ttyd::mario_party::marioPartyHello(4);
    }
  }
  else if (ttyd::string::strcmp(NextMap, "las_29") == 0)
  {
    // Warp out of the Shadow Queen room if she is defeated, so that the cutscene is skipped
    uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
    if (SequencePosition == 401)
    {
      if (LZRandoChallenge)
      {
        // Warp to a random room if the challenge mode is being used
        ttyd::swdrv::swByteSet(0, 405);
        ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "las_29", "w_bero");
      }
      else
      {
        // Warp to the credits
        ttyd::swdrv::swByteSet(0, 404);
        ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "end_00", nullptr);
      }
    }
  }
  else if (ttyd::string::strcmp(NextMap, "pik_02") == 0)
  {
    // Disable the pipe in the fake Garnet Star room
    *reinterpret_cast<uint32_t *>(AreaLZAddresses + 0x280) = 0;
  }
}

void dismountYoshi()
{
  // This needs to be possible in the event that a cutscene forces Mario off of Yoshi improperly
  uint32_t ButtonInput = ttyd::system::keyGetButton(0);
  uint16_t DismountYoshiCombo = PAD_L | PAD_Z;
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  
  bool ButtonComparison = (ButtonInput & DismountYoshiCombo) == DismountYoshiCombo;
  
  if (!ButtonComparison || (NextSeq != Game))
  {
    return;
  }
  
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

void resetMarioThroughLZ()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t MapChange = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kMapChange);
  
  // Prevent softlocks by resetting Mario back to normal upon entering a loading zone
  if (NextSeq != MapChange)
  {
    return;
  }
  
  uint16_t CurrentlyUsingTubeMode = 22;
  uint16_t CurrentlyUsingBoatMode = 25;
  uint16_t CurrentlyUsingYoshi = 26;
  
  ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
  
  bool TubeModeComparison = player->currentMotionId == CurrentlyUsingTubeMode;
  bool BoatModeComparison = player->currentMotionId == CurrentlyUsingBoatMode;
  bool UsingYoshiComparison = player->currentMotionId == CurrentlyUsingYoshi;
  
  if (TubeModeComparison || BoatModeComparison || UsingYoshiComparison)
  {
    player->currentMotionId = 0;
  }
}

void reloadScreen()
{
  uint32_t ButtonInput = ttyd::system::keyGetButton(0);
  uint16_t ReloadScreenCombo = PAD_L | PAD_B;
  
  if ((ButtonInput & ReloadScreenCombo) != ReloadScreenCombo)
  {
    return;
  }
  
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  int32_t MapChange = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kMapChange);
  
  if ((NextSeq < Game) || (NextSeq > MapChange))
  {
    return;
  }
  
  // Not in the pause menu
  // A separate address for NextBero is needed, as the original value will be cleared during the reloading process
  // The game will crash if NextMap is used directly in seqSetSeq, so a separate address must be used instead
  
  ttyd::string::strcpy(NewBero, NextBero);
  ttyd::string::strcpy(NewMap, NextMap);
  ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, NewMap, NewBero);
  ReloadCurrentScreen = true;
  
  uint32_t SystemLevel = ttyd::mariost::marioStGetSystemLevel();
  
  if (SystemLevel == 0)
  {
    return;
  }
  
  // Only run the following if the system level is not 0
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

void setNextBero()
{
  // Prevent changing the loading zone if currently in a room where using it would result in a softlock
  if (ttyd::string::strcmp(NextMap, "eki_03") == 0)
  {
    return;
  }
  
  uint32_t ButtonInput = ttyd::system::keyGetButton(0);
  uint16_t NBeroCombo = PAD_L | PAD_DPAD_UP;
  uint16_t SBeroCombo = PAD_L | PAD_DPAD_DOWN;
  uint16_t WBeroCombo = PAD_L | PAD_DPAD_LEFT;
  uint16_t EBeroCombo = PAD_L | PAD_DPAD_RIGHT;
  
  if ((ButtonInput & NBeroCombo) == NBeroCombo)
  {
    ttyd::string::strcpy(NextBero, "n_bero");
  }
  else if ((ButtonInput & SBeroCombo) == SBeroCombo)
  {
    ttyd::string::strcpy(NextBero, "s_bero");
  }
  else if ((ButtonInput & WBeroCombo) == WBeroCombo)
  {
    ttyd::string::strcpy(NextBero, "w_bero");
  }
  else if ((ButtonInput & EBeroCombo) == EBeroCombo)
  {
    ttyd::string::strcpy(NextBero, "e_bero");
  }
}

void failsafeCheats()
{
  marioNeverTransform();
  specificMapEdits();
  resetMarioThroughLZ();
  dismountYoshi();
  reloadScreen();
  setNextBero();
}

void resetValuesOnGameOver()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t GameOver = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGameOver);
    
  if (NextSeq != GameOver)
  {
    return;
  }
  
  ReloadCurrentScreen = false;
  GameOverFlag = true;
}

void reloadCurrentScreenFlag()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  
  if (ReloadCurrentScreen && (NextSeq == Game))
  {
    ReloadCurrentScreen = false;
  }
}

uint32_t getCurseReturnValue(uint32_t pouchCheckItem, const char *CheckMap)
{
  // Only run if the player doesn't already have the Boat Mode curse, and if the Loading Zone randomizer is in use
  if (!pouchCheckItem && LZRando)
  {
    // Only run if the player is currently in the room in Rogueport Sewers with the 3 shine sprites
    if (ttyd::string::strcmp(NextMap, CheckMap) == 0)
    {
      // Return arbitrary value greater than 0
      return 1;
    }
  }
  
  return pouchCheckItem;
}

extern "C" {
uint32_t enablePaperTubeModes(uint32_t pouchCheckItem)
{
  return getCurseReturnValue(pouchCheckItem, "jin_05");
}
}

extern "C" {
uint32_t enableBoatMode(uint32_t pouchCheckItem)
{
  return getCurseReturnValue(pouchCheckItem, "tik_20");
}
}

extern "C" {
int32_t fixMarioKeyOn(int32_t currentKeyValue)
{
  // Properly convert key to int8_t
  int8_t NewKeyValue = static_cast<int8_t>(currentKeyValue);
  
  // Prevent key from becoming negative
  if (NewKeyValue < 1)
  {
    return 0;
  }
  else
  {
    return NewKeyValue - 1;
  }
}
}

void Mod::preventPartyLeft(uint32_t id)
{
  // Prevent the game from removing partners
  // Only prevent from running if the Loading Zone randomizer is currently in use
  if (LZRando)
  {
    return;
  }
  else
  {
    // Call original function
    mPFN_partyLeft_trampoline(id);
  }
}

void Mod::writeLZRandoAssemblyPatches()
{
  #ifdef TTYD_US
    uint32_t RandomWarp = 0x800087C8;
    uint32_t TubeModeCheck = 0x80092344;
    uint32_t PaperModeCheck = 0x800A6A3C;
    uint32_t BoatModeCheck = 0x8009268C;
    uint32_t MarioKeyOn = 0x8005C1C8;
  #elif defined TTYD_JP
    uint32_t RandomWarp = 0x800086F0;
    uint32_t TubeModeCheck = 0x80090D90;
    uint32_t PaperModeCheck = 0x800A4DB4;
    uint32_t BoatModeCheck = 0x800910D8;
    uint32_t MarioKeyOn = 0x8005B370;
  #elif defined TTYD_EU
    uint32_t RandomWarp = 0x80008994;
    uint32_t TubeModeCheck = 0x800936A0;
    uint32_t PaperModeCheck = 0x800A7E0C;
    uint32_t BoatModeCheck = 0x800939E8;
    uint32_t MarioKeyOn = 0x8005C300;
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
  
  // Fix the marioKeyOn function to prevent the key from becoming negative
  patch::writeBranch(reinterpret_cast<void *>(MarioKeyOn), reinterpret_cast<void *>(StartMarioKeyOnFix));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackMarioKeyOnFix), reinterpret_cast<void *>(MarioKeyOn + 0x4));
}

void writeAdditionalLZRandoAssemblyPatches()
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
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Load = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kLoad);
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  int32_t Title = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kTitle);
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  
  if (!ClearCacheFlag && (NextSeq == Load) && (seqMainCheck == 4))
  {
    ClearCacheFlag = true;
    
    ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(WalkOnWater1), sizeof(uint32_t));
    ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(WalkOnWater1), sizeof(uint32_t));
    
    ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(WalkOnWater2), sizeof(uint32_t));
    ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(WalkOnWater2), sizeof(uint32_t));
    
    #ifdef TTYD_EU
      ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(WalkOnWater3), sizeof(uint32_t));
      ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(WalkOnWater3), sizeof(uint32_t));
    #endif
    
    ttyd::OSCache::DCFlushRange(reinterpret_cast<uint32_t *>(aaa_00_Address), 16);
    ttyd::OSCache::ICInvalidateRange(reinterpret_cast<uint32_t *>(aaa_00_Address), 16);
  }
  else if (ClearCacheFlag && ((NextSeq == Game) || (NextSeq == Title)))
  {
    ClearCacheFlag = false;
  }
}

void Mod::LZRandoStuff()
{
  // Only run if the Loading Zone Randomizer is being used
  if (LZRando)
  {
    setUpNewFile();
    failsafeCheats();
    resetValuesOnGameOver();
    reloadCurrentScreenFlag();
  }
  
  // Additional LZ Rando stuff that needs to run no matter what
  overwriteNewFileStrings();
  writeAdditionalLZRandoAssemblyPatches();
}

}