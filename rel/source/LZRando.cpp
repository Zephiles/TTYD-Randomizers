#include "mod.h"
#include "maps.h"
#include "items.h"
#include "buttons.h"
#include "patch.h"
#include "clearcache.h"

#include <ttyd/string.h>
#include <ttyd/swdrv.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/system.h>
#include <ttyd/npcdrv.h>
#include <ttyd/mario_party.h>
#include <ttyd/evt_pouch.h>
#include <ttyd/seqdrv.h>
#include <ttyd/mario.h>
#include <ttyd/mariost.h>
#include <ttyd/camdrv.h>
#include <ttyd/pmario_sound.h>
#include <ttyd/mario_cam.h>

#include <cstdio>

extern bool LZRandoChallenge;
extern uint32_t TimerCount;
extern bool LZRando;
extern bool ReloadCurrentScreen;
extern char *NextMap;
extern bool GameOverFlag;
extern char *NextBero;
extern char *NextArea;
extern bool NewFile;
extern uint32_t GSWAddressesStart;
extern uint32_t PossibleLZMaps[];
extern uint16_t LZMapArraySize;
extern uint32_t PossibleChallengeMaps[];
extern uint16_t ChallengeMapArraySize;
extern uint32_t seqMainAddress;
extern bool ClearCacheNewFileStrings;
// extern uint32_t AreaObjectsAddressesStart;
// extern uint32_t AreaLZsAddressesStart;
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
  void StartEnableWalkJumpOnWater();
  void StartEnableLandOnWater();
  void BranchBackEnableLandOnWater();
  void StartSpawnPartnerBattle();
  void BranchBackSpawnPartnerBattle();
  void StartResetFileLoadSpawn();
  void BranchBackResetFileLoadSpawn();
  void StartPreventBattleOnRespawn();
  void BranchBackPreventBattleOnRespawn();
}

namespace mod {

bool CheckChallengeModeTimerCutoff()
{
  uint32_t TimerCountCutoff = 144000; // 40 Minutes
  if (LZRandoChallenge && (TimerCount >= TimerCountCutoff))
  {
    return true;
  }
  else
  {
    return false;
  }
}

extern "C" {
void getRandomWarp()
{
  // This function is ran from a place that only runs once during a screen transition
  // Don't run if the Loading Zone randomizer is currently not in use
  if (!LZRando)
  {
    return;
  }
  
  uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  
  // Don't run if currently reloading the current screen
  if (ReloadCurrentScreen)
  {
    // If reloading the room with a Curse chest, make sure the chest is closed if the player has the Curse from it
    if (ttyd::string::strcmp(NextMap, "dou_07") == 0)
    {
      // Check if the Curse chest is open or not
      uint32_t CurstChest = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2EC);
      
      if (!(CurstChest & (1 << 4))) // Check if the 4 bit is off
      {
        // Manually close the chest if the player already has the curse
        if (ttyd::mario_pouch::pouchCheckItem(BoatModeCurse) > 0)
        {
          // Turn on GSWF(2980) to manually close the chest
          *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2EC) |= (1 << 4); // Turn on the 4 bit
        
          // Update the Sequence to what it would normally be after getting the curse
          ttyd::swdrv::swByteSet(0, 247);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_06") == 0)
    {
      // Check if the Curse chest is open or not
      uint32_t CurstChest = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x230);
      
      if (!(CurstChest & (1 << 21))) // Check if the 21 bit is off
      {
        // Manually close the chest if the player already has the curse
        if (ttyd::mario_pouch::pouchCheckItem(PaperModeCurse) > 0)
        {
          // Turn on GSWF(1493) to manually close the chest
          *reinterpret_cast<uint32_t *>(GSWAddresses + 0x230) |= (1 << 21); // Turn on the 21 bit
        
          // Update the Sequence to what it would normally be after getting the curse
          ttyd::swdrv::swByteSet(0, 45);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tik_19") == 0)
    {
      // Check if the Curse chest is open or not
      uint32_t CurstChest = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x220);
      
      if (!(CurstChest & (1 << 8))) // Check if the 8 bit is off
      {
        // Manually close the chest if the player already has the curse
        if (ttyd::mario_pouch::pouchCheckItem(PlaneModeCurse) > 0)
        {
          // Turn on GSWF(1352) to manually close the chest
          *reinterpret_cast<uint32_t *>(GSWAddresses + 0x220) |= (1 << 8); // Turn on the 8 bit
        
          // Update the Sequence to what it would normally be after getting the curse
          ttyd::swdrv::swByteSet(0, 15);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "usu_01") == 0)
    {
      // Check if the Curse chest is open or not
      uint32_t CurstChest = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x268);
      
      if (!(CurstChest & (1 << 11))) // Check if the 11 bit is off
      {
        // Manually close the chest if the player already has the curse
        if (ttyd::mario_pouch::pouchCheckItem(TubeModeCurse) > 0)
        {
          // Turn on GSWF(1931) to manually close the chest
          *reinterpret_cast<uint32_t *>(GSWAddresses + 0x268) |= (1 << 11); // Turn on the 11 bit
        
          // Update the Sequence to what it would normally be after getting the curse
          ttyd::swdrv::swByteSet(0, 187);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "mri_09") == 0)
    {
      // Check if the Blue Key chest is open or not
      uint32_t BlueKeyChest = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2DC); // GSWF(2852)
      
      if (!(BlueKeyChest & (1 << 4))) // Check if the 4 bit is off
      {
        // Manually close the chest if the player already has the Blue Key
        if (ttyd::mario_pouch::pouchCheckItem(BlueKey) > 0)
        {
          // Turn on GSWF(2852) to manually close the chest
          *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2DC) |= (1 << 4); // Turn on the 4 bit
          
          // Update the Sequence to what it would normally be after getting the Blue Key
          ttyd::swdrv::swByteSet(0, 98);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tou_01") == 0)
    {
      // Prevent the player from being able to get the Super Hammer multiple times
      if (SequencePosition == 139)
      {
        // Check if the player has the Super Hammer or not
        if (ttyd::mario_pouch::pouchCheckItem(SuperHammer) > 0)
        {
          // Update the Sequence to what it would normally be after getting the Super Hammer
          ttyd::swdrv::swByteSet(0, 140);
        }
      }
    }
    
    return;
  }
  
  bool title_comparison = ttyd::string::strcmp(NextMap, "title") == 0;
  
  // Don't run on the title screen
  if (title_comparison)
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
  if (SequencePosition == 0)
  {
    // Set flag to allow new file stuff to be set up
    NewFile = true;
    return;
  }
  
  GameOverFlag = false;
  bool ConfirmNewMap = false;
  
  // Get new map to warp to
  uint32_t *MapArray;
  uint16_t MapArraySize;
  
  if (LZRandoChallenge)
  {
    MapArray = PossibleChallengeMaps;
    MapArraySize = ChallengeMapArraySize;
  }
  else
  {
    MapArray = PossibleLZMaps;
    MapArraySize = LZMapArraySize;
  }
  
  while (!ConfirmNewMap)
  {
    char *NewRandomMap = reinterpret_cast<char *>(MapArray[ttyd::system::irand(MapArraySize)]);
    ttyd::string::strcpy(NextMap, NewRandomMap);
    ttyd::string::strncpy(NextArea, NewRandomMap, 3);
    
    if (ttyd::string::strcmp(NextMap, "aji_00") == 0)
    {
      // Skip the cutscenes and fight
      // Need to skip this cutscene and fight to prevent a crash
      if (SequencePosition < 361)
      {
        // Set the Sequence to 361 to prevent the cutscene from playing and to prevent the fight from occuring
        ttyd::swdrv::swByteSet(0, 361);
      }
    }
    else if (ttyd::string::strcmp(NextMap, "aji_02") == 0)
    {
      // Change the loading zone to prevent spawning on the electric floor
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "aji_05") == 0)
    {
      // Change the loading zone to prevent spawning on the electric floor
      ttyd::string::strcpy(NextBero, "e_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "aji_07") == 0)
    {
      // Set the Loading Zone to be able to get the Cog if the player doesn't have it already
      uint32_t CogFlag = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x384); // GSWF(4194)
      
      if (!(CogFlag & (1 << 2))) // Check if the 2 bit is off
      {
        ttyd::string::strcpy(NextBero, "tenjo");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "aji_12") == 0)
    {
      // Change the loading zone to prevent spawning on the electric floor
      ttyd::string::strcpy(NextBero, "e_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "aji_14") == 0)
    {
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 373
      if ((SequencePosition < 373) && CheckChallengeModeTimerCutoff())
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "bom_03") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "bom_04") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "e_bero4");
    }
    else if (ttyd::string::strcmp(NextMap, "dou_02") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "w_bero_1");
    }
    else if (ttyd::string::strcmp(NextMap, "eki_03") == 0)
    {
      // Change loading zone to avoid softlocking
      ttyd::string::strcpy(NextBero, "w_bero_1");
    }
    else if (ttyd::string::strcmp(NextMap, "eki_04") == 0)
    {
      // Change the loading zone to prevent spawning in the tube maze
      ttyd::string::strcpy(NextBero, "s_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "eki_05") == 0)
    {
      // Change the loading zone used if the player has not opened the Ultra Boots chest yet, and if the Sequence is less than 319
      uint32_t UltraBootsChest = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x348); // GSWF(3728)
      
      if ((!(UltraBootsChest & (1 << 16))) && (SequencePosition < 319)) // Check if the 16 bit is off
      {
        // Run if the 16 bit is off
        ttyd::string::strcpy(NextBero, "w_bero_1");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_01") == 0)
    {
      if (LZRandoChallenge)
      {
        // Skip the intro cutscene if using the challenge mode
        if (SequencePosition < 37)
        {
          // Set the Sequence to 37 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 37);
        }
      }      
    }
    else if (ttyd::string::strcmp(NextMap, "gon_03") == 0)
    {
      if (LZRandoChallenge)
      {
        // Skip the cutscene with Koops using the challenge mode
        if (SequencePosition < 38)
        {
          // Set the Sequence to 38 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 38);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_05") == 0)
    {
      // Randomize the entrance used
      bool LowerWestEntranceCheck = ttyd::system::irand(1000) < 500;
      
      if (LowerWestEntranceCheck)
      {
        ttyd::string::strcpy(NextBero, "w_bero_2");
      }
      else
      {
        ttyd::string::strcpy(NextBero, "w_bero_4");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_06") == 0)
    {
      // Adjust the Sequence and GSWF(1493) if the player hasn't gotten the Paper Curse yet
      if (ttyd::mario_pouch::pouchCheckItem(PaperModeCurse) == 0)
      {
        // Only run if the player has the Black Key
        if (ttyd::mario_pouch::pouchCheckItem(BlackKey2) > 0)
        {
          // Adjust the Sequence if necessary, as the Sequence being past 44 will set GSWF(1493)
          if (SequencePosition > 44)
          {
            ttyd::swdrv::swByteSet(0, 44);
          }
          
          // Turn off GSWF(1493)
          *reinterpret_cast<uint32_t *>(GSWAddresses + 0x230) &= ~(1 << 21); // Turn off the 21 bit
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_07") == 0)
    {
      // Adjust the Sequence and GSWF(1494) if the player hasn't gotten the Paper Curse yet
      if (ttyd::mario_pouch::pouchCheckItem(PaperModeCurse) == 0)
      {
        // Only run if the player hasn't gotten the Black Key yet
        if (ttyd::mario_pouch::pouchCheckItem(BlackKey2) == 0)
        {
          // Adjust the Sequence if necessary, as the Sequence being past 42 will set GSWF(1494)
          if (SequencePosition > 42)
          {
            ttyd::swdrv::swByteSet(0, 42);
          }
          
          // Turn off GSWF(1494)
          *reinterpret_cast<uint32_t *>(GSWAddresses + 0x230) &= ~(1 << 22); // Turn off the 22 bit
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_08") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "e_bero_1");
    }
    else if (ttyd::string::strcmp(NextMap, "gon_11") == 0)
    {
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 56
      if ((SequencePosition < 56) && CheckChallengeModeTimerCutoff())
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_12") == 0)
    {
      if (LZRandoChallenge)
      {
        // Skip the Mowz cutscene if using the challenge mode
        if (SequencePosition < 50)
        {
          // Set the Sequence to 50 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 50);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gor_04") == 0)
    {
      // Set the Loading Zone to south
      ttyd::string::strcpy(NextBero, "s_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "gra_01") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "w_bero1");
    }
    else if (ttyd::string::strcmp(NextMap, "gra_02") == 0)
    {
      // Change the loading zone to prevent the player from being forced to update the Sequence to leave the room
      ttyd::string::strcpy(NextBero, "w_bero_1");
    }
    else if (ttyd::string::strcmp(NextMap, "gra_03") == 0)
    {
      // Change the loading zone to prevent falling through the floor
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "gra_06") == 0)
    {
      if (LZRandoChallenge)
      {
        // Skip the intro cutscene if using the challenge mode
        if (SequencePosition < 193)
        {
          // Set the Sequence to 193 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 193);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "hei_00") == 0)
    {
      if (LZRandoChallenge)
      {
        // Change the loading zone to prevent falling through the floor
        ttyd::string::strcpy(NextBero, "dokan");
        
        // Skip the intro cutscene and the Hooktail flying cutscene if using the challenge mode
        if (SequencePosition < 24)
        {
          // Set the Sequence to 24 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 24);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "hei_02") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "hei_04") == 0)
    {
      // Change the loading zone to prevent falling through the floor
      ttyd::string::strcpy(NextBero, "e_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "hei_08") == 0)
    {
      // Change the loading zone to prevent spawning on the stone or enemy
      ttyd::string::strcpy(NextBero, "e_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "hei_12") == 0)
    {
      // Change the loading zone to prevent spawning on the stone or enemy
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "hei_13") == 0)
    {
      // Change the loading zone to prevent spawning on the Paragoomba
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "hom_00") == 0)
    {
      if (LZRandoChallenge)
      {
        // Skip the cutscene with Doopliss if using the challenge mode
        if (SequencePosition < 310)
        {
          // Set the Sequence to 310 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 310);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "jin_00") == 0)
    {
      if (CheckChallengeModeTimerCutoff())
      {
        if (ttyd::mario_pouch::pouchGetHammerLv() > 1)
        {
          // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the player has a Hammer upgrade
          continue;
        }
        else if (SequencePosition < 194)
        {
          // Adjust the Sequence to skip the intro cutscene
          // Set the Sequence to 194 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 194);
        }
      }
      else if (LZRandoChallenge)
      {
        // Adjust the Sequence to skip the intro cutscene
        if (SequencePosition < 194)
        {
          // Set the Sequence to 194 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 194);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "jin_04") == 0)
    {
      bool CheckChallengeModeTime = CheckChallengeModeTimerCutoff();
      
      // Check if the sequence is before or at Doopliss 2
      if (SequencePosition <= 210)
      {
        // Check if the Sequence is before or at Doopliss 1
        if (SequencePosition <= 199)
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          if (CheckChallengeModeTime)
          {
            continue;
          }
          else
          {
            // Allow Doopliss 1 or 2 to be fought at random
            bool DooplissCheck = ttyd::system::irand(1000) < 500;
            
            if (DooplissCheck)
            {
              // Set up the Sequence to fight Doopliss 1
              ttyd::swdrv::swByteSet(0, 199);
            }
            else
            {
              // Set up the Sequence to fight Doopliss 2
              ttyd::swdrv::swByteSet(0, 210);
            }
          }
        }
        else // Sequence is after Doopliss 1 but before or at Doopliss 2
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          if (CheckChallengeModeTime)
          {
            continue;
          }
          else
          {
            // Set up the Sequence to fight Doopliss 2
            ttyd::swdrv::swByteSet(0, 210);
          }
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "jin_07") == 0)
    {
      // Change the loading zone to allow the player to get the Star Piece
      ttyd::string::strcpy(NextBero, "w_bero_2");
    }
    else if (ttyd::string::strcmp(NextMap, "jin_10") == 0)
    {
      // Change the loading zone to prevent spawning near the enemies
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "jon_00") == 0)
    {
      // Prevent warping to the Pit if 10 minutes have not passed
      uint32_t TimerCountCutoff = 180000; // 50 Minutes
      if (TimerCount >= TimerCountCutoff)
      {
        continue;
      }
      
      // Adjust Pit floor if a Pit room is selected
      uint32_t TotalPitFloors = 100;
      
      if (CheckChallengeModeTimerCutoff())
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
    else if (ttyd::string::strcmp(NextMap, "las_03") == 0)
    {
      // Change the loading zone to prevent spawning on the spikes
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "las_04") == 0)
    {
      // Change the loading zone to prevent spawning under the map
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "las_09") == 0)
    {
      if (SequencePosition <= 390)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow the Shadow Sirens to be fought if the Sequence is before or at 390
          // Set the Sequence to 390 so that the Shadow Sirens can be fought
          ttyd::swdrv::swByteSet(0, 390);
          ttyd::string::strcpy(NextBero, "e_bero");
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_19") == 0)
    {
      if (SequencePosition < 390)
      {
        // The room changes once the Sequence reaches 390
        // Change the loading zone to prevent spawning on the Wizzerd
        ttyd::string::strcpy(NextBero, "w_bero");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_26") == 0)
    {
      if ((SequencePosition <= 387) && !CheckChallengeModeTimerCutoff())
      {
        // Allow Gloomtail to be fought if the Sequence is before or at 387
        // Set the Sequence to 387 so that Gloomtail can be fought
        ttyd::swdrv::swByteSet(0, 387);
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_29") == 0)
    {
      if (SequencePosition <= 400)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow the Shadow Queen to be fought if the Sequence is before or at 400
          // Set the Sequence to 400 so that the Shadow Queen can be fought
          ttyd::swdrv::swByteSet(0, 400);
          ttyd::string::strcpy(NextBero, "sekai_yami2");
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "moo_00") == 0)
    {
      // Skip the intro cutscene
      // Need to skip this cutscene to prevent a crash
      if (SequencePosition < 358)
      {
        // Set the Sequence to 358 to prevent the cutscene from playing
        ttyd::swdrv::swByteSet(0, 358);
      }
    }
    else if (ttyd::string::strcmp(NextMap, "moo_00") == 0)
    {
      // Change the loading zone to prevent spawning on the Yux
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "mri_01") == 0)
    {
      if (SequencePosition <= 110)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Magnus 1.0 to be fought if the Sequence is before or at 110
          // Set the Sequence to 110 so that Magnus 1.0 can be fought
          ttyd::swdrv::swByteSet(0, 110);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "muj_12") == 0)
    {
      if (SequencePosition <= 252)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Cortez to be fought if the Sequence is before or at 252
          // Set the Sequence to 252 so that Cortez can be fought
          ttyd::swdrv::swByteSet(0, 252);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "nok_00") == 0)
    {
      if (SequencePosition < 26)
      {
        if (LZRandoChallenge)
        {
          // Skip the intro cutscene
          // Set the Sequence to 26 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 26);
        }
        else
        {
          // Clear a pointer to prevent a crash with the intro cutscene
          uint32_t fbatPreviousInteractionPointer = reinterpret_cast<uint32_t>(ttyd::npcdrv::fbatGetPointer());
          *reinterpret_cast<uint32_t *>(fbatPreviousInteractionPointer + 0x4) = 0;
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
      if ((SequencePosition < 332) && CheckChallengeModeTimerCutoff())
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tik_02") == 0)
    {
      if ((SequencePosition <= 20) && !CheckChallengeModeTimerCutoff())
      {
        // Allow Blooper to be fought if the Sequence is before or at 20
        // Set the Sequence to 20 so that Blooper can be fought
        ttyd::swdrv::swByteSet(0, 20);
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tik_07") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "n_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "tik_15") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "w_bero_2");
    }
    else if (ttyd::string::strcmp(NextMap, "tik_21") == 0)
    {
      // Change the loading zone to prevent spawning on the spikes
      ttyd::string::strcpy(NextBero, "s_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "tou_01") == 0)
    {
      // Adjust the Sequence to allow the player to get the Super Hammer if they don't have it already
      if (ttyd::mario_pouch::pouchCheckItem(SuperHammer) == 0)
      {
        // Set the Sequence to 139 so that the player can get the Super Hammer
        ttyd::swdrv::swByteSet(0, 139);
        
        if (LZRandoChallenge)
        {
          // Change the loading zone used to skip the cutscene of coming off of the blimp if using the challenge mode
          ttyd::string::strcpy(NextBero, "a_door_mon");
        }
      }
      else if (SequencePosition == 139)
      {
        // Set the Sequence to 140 so that the player can't get the Super Hammer again
        ttyd::swdrv::swByteSet(0, 140);
        
        if (LZRandoChallenge)
        {
          // Change the loading zone used to skip the cutscene of coming off of the blimp if using the challenge mode
          ttyd::string::strcpy(NextBero, "a_door_mon");
        }
      }
      else if (LZRandoChallenge)
      {
        if (SequencePosition < 127)
        {
          // Set the Sequence to 127 to prevent the intro cutscene from playing
          ttyd::swdrv::swByteSet(0, 127);
        }
        
        // Change the loading zone used to skip the cutscene of coming off of the blimp if using the challenge mode
        ttyd::string::strcpy(NextBero, "a_door_mon");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tou_03") == 0)
    {
      if (SequencePosition <= 163)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Grubba to be fought if the Sequence is before or at 163
          // Set the Sequence to 163 so that Grubba can be fought
          ttyd::swdrv::swByteSet(0, 163);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tou_04") == 0)
    {
      // Change the loading zone to prevent spawning out of bounds
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "tou_05") == 0)
    {
      if (LZRandoChallenge)
      {
        // Adjust the Sequence to skip the cutscene of signing up to be a fighter if using the challenge mode
        if (SequencePosition < 130)
        {
          // Set the Sequence to 130 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 130);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tou_06") == 0)
    {
      if (LZRandoChallenge)
      {
        // Adjust the Sequence to skip the Mowz cutscene if using the challenge mode
        if (SequencePosition < 148)
        {
          // Set the Sequence to 148 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 148);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "usu_00") == 0)
    {
      if (LZRandoChallenge)
      {
        // Adjust the Sequence to skip the intro cutscene if using the challenge mode
        if (SequencePosition < 178)
        {
          // Set the Sequence to 178 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 178);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "win_00") == 0)
    {
      if (SequencePosition <= 84)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Currently using the challenge mode and 20 minutes have not passed, so don't set the sequence for the Shadow Sirens
          if (SequencePosition == 84)
          {
            // Prevent fighting the Shadow Sirens if the Sequence is exactly 84
            continue;
          }
          else if (SequencePosition < 72)
          {
            // Adjust the Sequence to skip the cutscene with the Shadow Sirens if using the challenge mode
            // Set the Sequence to 72 to prevent the cutscene from playing
            ttyd::swdrv::swByteSet(0, 72);
          }
        }
        else
        {
          // Allow the Shadow Sirens to be fought if the Sequence is before or at 84
          // Set the Sequence to 84 so that the Shadow Sirens can be fought
          ttyd::swdrv::swByteSet(0, 84);
          ttyd::string::strcpy(NextBero, "w_bero");
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "win_01") == 0)
    {
      if (LZRandoChallenge)
      {
        // Adjust the Sequence to skip the cutscene with Punio going through the pipe to the Great Tree
        if (SequencePosition < 73)
        {
          // Set the Sequence to 73 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 73);
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
    ttyd::mario_party::partyJoin(static_cast<ttyd::party::Party>(i));
  }
  
  // Reset party stats to what they normally would be
  ttyd::mario_pouch::pouchRevisePartyParam();
  
  // Heal all party members, as they will not be healed by pouchRevisePartyParam
  ttyd::evt_pouch::evt_pouch_all_party_recovery();
  
  // Randomize Yoshi color
  ttyd::party::yoshiSetColor();
  
  // Start with Yoshi out
  ttyd::mario_party::marioPartyHello(ttyd::party::Party::Yoshi);
  
  // Start with the Strange Sack
  ttyd::mario_pouch::pouchGetItem(StrangeSack);
  
  // Turn off a bit to enable loading zones
  uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
  *reinterpret_cast<uint32_t *>(GSWAddresses) &= ~(1 << 0); // Turn off the 0 bit
  
  // Turn on GSWF(0) to skip shop tutorials
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x178) |= (1 << 0); // Turn on the 0 bit
  
  // Turn on GSWF(37) and GSWF(38) to prevent emails from being explained for the first time
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x17C) |= ((1 << 5) | (1 << 6)); // Turn on the 5 and 6 bits
  
  // Turn on GSWF(233) to skip save blocks from being explained for the first time
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x194) |= (1 << 9); // Turn on the 9 bit
  
  // Turn on GSWF(234) to skip recovery blocks from being explained for the first time
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x194) |= (1 << 10); // Turn on the 10 bit
  
  // Turn on GSWF(235) to skip items from being explained for the first time
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x194) |= (1 << 11); // Turn on the 11 bit
  
  // Turn on GSWF(1187) to set Zess T. to be blocking the west entrance
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x20C) |= (1 << 3); // Turn on the 3 bit
  
  // Turn on GSWF(1189) to have the Contact Lens ordered already
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x20C) |= (1 << 5); // Turn on the 5 bit
  
  // Turn on GSWF(1197) to skip Zess T. explaining that he will cook stuff for the player now
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x20C) |= (1 << 13); // Turn on the 13 bit
  
  // Turn on GSWF(1200) to prevent partners from explaining Save Blocks in central Rogueport
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x20C) |= (1 << 16); // Turn on the 16 bit
  
  // Turn on GSWF(1334) to have the entrances revealed already in tik_03
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x21C) |= (1 << 22); // Turn on the 22 bit
  
  // Turn on GSWF(1781) and GSWF(1782) to skip the Koopie Koo cutscene in Petal Meadows
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x254) |= ((1 << 21) | (1 << 22)); // Turn on the 21 and 22 bits
  
  // Turn on GSWF(1805) to skip the cutscene of Goombella explaining her tattles on the bridge screen in Petal Meadows
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x258) |= (1 << 13); // Turn on the 13 bit
  
  // Turn on GSWF(2075) to skip Vivian's textbox in Twilight Trail
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x278) |= (1 << 27); // Turn on the 27 bit
  
  // Turn on GSWF(2401) to skip the cutscene of entering Grubba's office through the grate for the first time
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2A4) |= (1 << 1); // Turn on the 1 bit
  
  // Turn on GSWF(2500) to skip the cutscene of re-signing up to be a fighter
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2B0) |= (1 << 4); // Turn on the 4 bit
  
  // Turn on GSWF(2867) to drain the water in the Great Tree
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2DC) |= (1 << 19); // Turn on the 19 bit
  
  // Turn on GSWF(2878) to prevent the player from being able to talk to Jabble
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2DC) |= (1 << 30); // Turn on the 30 bit
  
  // Turn on GSWF(2982), GSWF(2983), and GSWF(2984) to activate the blue switches in Pirate's Grotto
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2EC) |= ((1 << 6) | (1 << 7) | (1 << 8)); // Turn on the 6, 7, and 8 bits
  
  // Turn on GSWF(3131) to skip the cutscene with Four-Eyes after Bobbery is taken by the Embers
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x2FC) |= (1 << 27); // Turn on the 27 bit
  
  // Turn on GSWF(3884) to skip the cutscene on the first screen of the area leading to Fahr Outpost
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x35C) |= (1 << 12); // Turn on the 12 bit
  
  // Turn on GSWF(4196) and GSWF(4197) to allow the player to use the crane without needing to insert the Cog
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x384) |= ((1 << 4) | (1 << 5)); // Turn on the 4 and 5 bits
  
  // Turn on GSWF(4218) to skip the crane game tutorial
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x384) |= (1 << 26); // Turn on the 26 bit
  
  // Turn on GSWF(5374) to skip the Trouble Center tutorial
  *reinterpret_cast<uint32_t *>(GSWAddresses + 0x414) |= (1 << 30); // Turn on the 30 bit
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
    clearcache::clearCache(aaa_00_Address, 16);
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

uint8_t getPartnerOut(uint32_t PartnerPointer)
{
  if (PartnerPointer != 0)
  {
    return *reinterpret_cast<uint8_t *>(PartnerPointer + 0x31);
  }
  else
  {
    return 0;
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
  
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  uint32_t PartnerPointer = reinterpret_cast<uint32_t>(ttyd::party::partyGetPtr(ttyd::mario_party::marioGetPartyId()));
  // uint32_t AreaObjectAddresses = *reinterpret_cast<uint32_t *>(AreaObjectsAddressesStart + 0x4);
  // uint32_t AreaLZAddresses = *reinterpret_cast<uint32_t *>(AreaLZsAddressesStart);
  // AreaLZAddresses = *reinterpret_cast<uint32_t *>(AreaLZAddresses + 0x4);
  
  if (ttyd::string::strcmp(NextMap, "gon_12") == 0)
  {
    if (SequencePosition < 50)
    {
      uint8_t CurrentPartnerOut = getPartnerOut(PartnerPointer);
      
      if ((CurrentPartnerOut == 0) || (CurrentPartnerOut > static_cast<uint8_t>(ttyd::party::Party::Koops)))
      {
        // Bring out Goombella if either no partner is out or a partner from past Chapter 1 is out
        ttyd::mario_party::marioPartyEntry(ttyd::party::Party::Goombella);
      }
    }
  }
  else if (ttyd::string::strcmp(NextMap, "jin_04") == 0)
  {
    // Check if the Sequence is currently set to fight Doopliss 2
    if ((SequencePosition == 210) || (SequencePosition == 211))
    {
      uint8_t CurrentPartnerOut = getPartnerOut(PartnerPointer);
      
      if (CurrentPartnerOut != static_cast<uint8_t>(ttyd::party::Party::Vivian))
      {
        // Force Vivian out
        ttyd::mario_party::marioPartyEntry(ttyd::party::Party::Vivian);
      }
    }
  }
  else if (ttyd::string::strcmp(NextMap, "las_29") == 0)
  {
    // Make sure a partner is out for the Shadow Queen cutscene
    if (!PartnerPointer)
    {
      // Bring out Goombella if no partner is out
      ttyd::mario_party::marioPartyHello(ttyd::party::Party::Goombella);
    }
    
    // Warp out of the Shadow Queen room if she is defeated, so that the cutscene is skipped
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
  
  int32_t Seq = ttyd::seqdrv::seqGetSeq();
  int32_t Battle = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kBattle);
  
  if (Seq == Battle)
  {
    // Reloading the room with the Seq set to Battle will cause the game to crash, so don't allow it
    return;
  }
  
  // Prevent reloading the room if currently getting an item. This prevents the player from reloading the room after opening a chest in order to respawn it closed, while at the same time keeping the item from it.
  ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
  uint16_t ReceivingItem = 15;
  
  if (player->currentMotionId == ReceivingItem)
  {
    return;
  }
  
  // Prevent reloading the room if the player is currently in a room with a boss. This prevents the player from repeatedly killing the boss for SP and/or points.
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  if (ttyd::string::strcmp(NextMap, "aji_14") == 0)
  {
    if (SequencePosition < 373)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "gon_11") == 0)
  {
    if (SequencePosition < 55)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "las_09") == 0)
  {
    if (SequencePosition == 390)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "las_26") == 0)
  {
    if (SequencePosition == 387)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "las_29") == 0)
  {
    if (SequencePosition == 400)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "muj_12") == 0)
  {
    if (SequencePosition == 252)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "rsh_06_a") == 0)
  {
    if (SequencePosition < 332)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "tik_02") == 0)
  {
    if (SequencePosition == 20)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "tou_03") == 0)
  {
    if (SequencePosition == 163)
    {
      return;
    }
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
  
  // Prevent changing the loading zone for rooms that need it to be set to a specific value
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  if (ttyd::string::strcmp(NextMap, "aji_14") == 0)
  {
    if (SequencePosition < 373)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(NextMap, "las_29") == 0)
  {
    if (SequencePosition == 400)
    {
      return;
    }
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
  
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  if (SequencePosition == 111)
  {
    // Adjust the Sequence if the player got a Game Over on Magnus 1. This will prevent the Emerald Star cutscene from playing if they happen to warp back there
    // Set the Sequence back one
    ttyd::swdrv::swByteSet(0, 110);
  }
  else if (SequencePosition == 400)
  {
    // Adjust the Sequence if the player got a Game Over on the Shadow Queen. This will prevent warping back into the cutscene for her.
    // Set the Sequence to an arbitary value right before anything major
    ttyd::swdrv::swByteSet(0, 393);
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

extern "C" {
bool enableWalkOnWater(uint32_t CheckBit)
{
  // Allow if the Loading Zone randomizer is in use
  if (LZRando)
  {
    return false;
  }
  else
  {
    return (CheckBit & (1 << 9)); // Check if the 9 bit is on
  }
}
}

extern "C" {
uint32_t spawnPartnerBattle(uint32_t PartnerValue)
{
  // Force a partner out of one isn't already
  // Only force a partner out if the Loading Zone randomizer is in use
  if ((PartnerValue == 0) && LZRando)
  {
    uint32_t Goombella = 224;
    
    // Check if fighting Doopliss 2
    if (ttyd::string::strcmp(NextMap, "jin_04") == 0)
    {
      uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
      if (SequencePosition == 210)
      {
        // Don't bring out a partner if currently fighting Doopliss 2
        return 0;
      }
      else
      {
        // Not fighting Doopliss 2, so bring out Goombella
        return Goombella;
      }
    }
    else
    {
      // Bring out Goombella
      return Goombella;
    }
  }
  
  return PartnerValue;
}
}

extern "C" {
void *preventBattleOnRespawn(void *fbatPointer)
{
  // The reloadScreen function runs too late for this, so the button combo has to be checked
  uint32_t ButtonInput = ttyd::system::keyGetButton(0);
  uint16_t ReloadScreenCombo = PAD_L | PAD_B;
  
  if ((ButtonInput & ReloadScreenCombo) == ReloadScreenCombo)
  {
    return nullptr;
  }
  else
  {
    return fbatPointer;
  }
}
}

extern "C" {
void resetFileLoadSpawn(void *GSWAddressesPointer)
{
  // Only reset if using the Loading Zone randomizer
  if (LZRando)
  {
    uint32_t GSWAddresses = reinterpret_cast<uint32_t>(GSWAddressesPointer);
    
    *reinterpret_cast<float *>(GSWAddresses + 0x11D4) = 0; // Coordinate X
    *reinterpret_cast<float *>(GSWAddresses + 0x11D8) = 0; // Coordinate Y
    *reinterpret_cast<float *>(GSWAddresses + 0x11DC) = 0; // Coordinate Z
  }
}
}

void Mod::preventPartyLeft(ttyd::party::Party id)
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

int32_t Mod::randomizeGivenFollower(ttyd::party::Party id, float x, float y, float z)
{
  // Randomize given followers
  // Only randomize if the Loading Zone randomizer is currently in use
  if (LZRando)
  {
    id = static_cast<ttyd::party::Party>(ttyd::system::irand(5) + 8);
  }
  
  // Call original function
  return mPFN_randomizeGivenFollower_trampoline(id, x, y, z);
}

void Mod::preventCountDownStart(uint32_t unk1, uint32_t unk2)
{
  // Only prevent from running if the Loading Zone randomizer is currently in use
  if (LZRando)
  {
    return;
  }
  else
  {
    // Call original function
    mPFN_countDownStart_trampoline(unk1, unk2);
  }
}

void *Mod::preventMarioShipForceStop()
{
  // Only prevent from running if the Loading Zone randomizer is currently in use
  if (LZRando)
  {
    return nullptr;
  }
  else
  {
    // Call original function
    return mPFN_marioShipForceStop_trampoline();
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
    uint32_t WalkOnWater = 0x8008F940;
    uint32_t LandOnWater = 0x80092DF4;
    uint32_t SpawnPartnerInBattle = 0x800F8B44;
    uint32_t ResetFileLoadCoordinates = 0x800F3A1C;
    uint32_t PreventBattleOnRespawn = 0x800465CC;
  #elif defined TTYD_JP
    uint32_t RandomWarp = 0x800086F0;
    uint32_t TubeModeCheck = 0x80090D90;
    uint32_t PaperModeCheck = 0x800A4DB4;
    uint32_t BoatModeCheck = 0x800910D8;
    uint32_t MarioKeyOn = 0x8005B370;
    uint32_t WalkOnWater = 0x8008E38C;
    uint32_t LandOnWater = 0x80091840;
    uint32_t SpawnPartnerInBattle = 0x800F3BF4;
    uint32_t ResetFileLoadCoordinates = 0x800EED60;
    uint32_t PreventBattleOnRespawn = 0x80045F28;
  #elif defined TTYD_EU
    uint32_t RandomWarp = 0x80008994;
    uint32_t TubeModeCheck = 0x800936A0;
    uint32_t PaperModeCheck = 0x800A7E0C;
    uint32_t BoatModeCheck = 0x800939E8;
    uint32_t MarioKeyOn = 0x8005C300;
    uint32_t WalkOnWater = 0x80090C9C;
    uint32_t LandOnWater = 0x80094170;
    uint32_t JumpOnWater = 0x80093CFC;
    uint32_t SpawnPartnerInBattle = 0x800F99B0;
    uint32_t ResetFileLoadCoordinates = 0x800F4888;
    uint32_t PreventBattleOnRespawn = 0x800466B4;
  #endif
  
  // Random Warps
  patch::writeBranch(reinterpret_cast<void *>(RandomWarp), reinterpret_cast<void *>(StartGetRandomWarp));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackGetRandomWarp), reinterpret_cast<void *>(RandomWarp + 0x4));
  
  // Allow Paper and Tube modes to be used in the Creepy Steeple room with the Cook Book
  patch::writeBranch(reinterpret_cast<void *>(TubeModeCheck), reinterpret_cast<void *>(StartEnablePaperTubeModes));
  patch::writeBranch(reinterpret_cast<void *>(PaperModeCheck), reinterpret_cast<void *>(StartEnablePaperTubeModes));
  
  // Adjust branches to be bl instead of b; should modify the patch function to allow for this instead
  *reinterpret_cast<uint32_t *>(TubeModeCheck) += 0x1;
  *reinterpret_cast<uint32_t *>(PaperModeCheck) += 0x1;
  
  // Allow Boat mode to be used in the Rogueport Sewers room with the 3 Shine Sprites
  patch::writeBranch(reinterpret_cast<void *>(BoatModeCheck), reinterpret_cast<void *>(StartEnableBoatMode));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackEnableBoatMode), reinterpret_cast<void *>(BoatModeCheck + 0x4));
  
  // Fix the marioKeyOn function to prevent the key from becoming negative
  patch::writeBranch(reinterpret_cast<void *>(MarioKeyOn), reinterpret_cast<void *>(StartMarioKeyOnFix));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackMarioKeyOnFix), reinterpret_cast<void *>(MarioKeyOn + 0x4));
  
  // Allow Walking, Landing, and Jumping on water
  patch::writeBranch(reinterpret_cast<void *>(LandOnWater), reinterpret_cast<void *>(StartEnableLandOnWater));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackEnableLandOnWater), reinterpret_cast<void *>(LandOnWater + 0x4));
  patch::writeBranch(reinterpret_cast<void *>(WalkOnWater), reinterpret_cast<void *>(StartEnableWalkJumpOnWater));
  #ifdef TTYD_EU
    patch::writeBranch(reinterpret_cast<void *>(JumpOnWater), reinterpret_cast<void *>(StartEnableWalkJumpOnWater));
  #endif
  
  // Adjust branches to be bl instead of b; should modify the patch function to allow for this instead
  *reinterpret_cast<uint32_t *>(WalkOnWater) += 0x1;
  #ifdef TTYD_EU
    *reinterpret_cast<uint32_t *>(JumpOnWater) += 0x1;
  #endif
  
  // Force a partner out in battle if entering without one out
  patch::writeBranch(reinterpret_cast<void *>(SpawnPartnerInBattle), reinterpret_cast<void *>(StartSpawnPartnerBattle));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackSpawnPartnerBattle), reinterpret_cast<void *>(SpawnPartnerInBattle + 0x4));
  
  // Reset Mario's coordinates upon loading a file
  patch::writeBranch(reinterpret_cast<void *>(ResetFileLoadCoordinates), reinterpret_cast<void *>(StartResetFileLoadSpawn));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackResetFileLoadSpawn), reinterpret_cast<void *>(ResetFileLoadCoordinates + 0x4));
  
  // Prevent battles occuring when reloading the room
  patch::writeBranch(reinterpret_cast<void *>(PreventBattleOnRespawn), reinterpret_cast<void *>(StartPreventBattleOnRespawn));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackPreventBattleOnRespawn), reinterpret_cast<void *>(PreventBattleOnRespawn + 0x4));
}

void writeAdditionalLZRandoAssemblyPatches()
{
  #ifdef TTYD_US
    uint32_t aaa_00_Address = 0x802EDE78;
  #elif defined TTYD_JP
    uint32_t aaa_00_Address = 0x802ED930;
  #elif defined TTYD_EU
    uint32_t aaa_00_Address = 0x802F9AD8;
  #endif
  
  // Clear Cache
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Load = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kLoad);
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  int32_t Title = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kTitle);
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  
  if (!ClearCacheFlag && (NextSeq == Load) && (seqMainCheck == 4))
  {
    ClearCacheFlag = true;
    clearcache::clearCache(aaa_00_Address, 16);
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