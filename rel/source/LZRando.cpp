#include "mod.h"
#include "maps.h"
#include "items.h"
#include "buttons.h"
#include "patch.h"

#include <ttyd/string.h>
#include <ttyd/swdrv.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/system.h>
#include <ttyd/npcdrv.h>
#include <ttyd/mario_party.h>
#include <ttyd/evt_pouch.h>
#include <ttyd/OSCache.h>
#include <ttyd/seqdrv.h>
#include <ttyd/mario.h>
#include <ttyd/imgdrv.h>
#include <ttyd/mariost.h>
#include <ttyd/camdrv.h>
#include <ttyd/pmario_sound.h>
#include <ttyd/mario_cam.h>

#include <cstdio>

extern bool LZRandoChallenge;
extern uint32_t TimerCount;
extern bool LZRando;
extern uint32_t GSWAddressesStart;
extern bool ReloadCurrentScreen;
extern char *NextMap;
extern bool GameOverFlag;
extern char *NextBero;
extern char *NextArea;
extern bool NewFile;
extern bool MarioFreeze;
extern uint32_t _mapEntAddress;
extern uint32_t PossibleChallengeMaps[];
extern uint16_t ChallengeMapArraySize;
extern uint32_t PossibleLZMaps[];
extern uint16_t LZMapArraySize;
extern uint32_t seqMainAddress;
extern bool ClearCacheFlag;
extern bool SQWarpAway;
extern uint8_t JustDefeatedBoss;
extern uint32_t imgEntryAddress;
// extern uint32_t AreaObjectsAddressesStart;
// extern uint32_t AreaLZsAddressesStart;
// extern uint32_t NPCAddressesStart;
extern char *NewBero;
extern char *NewMap;
extern uint32_t wp_fadedrv_Address;

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
  void StartCheckCurrentTextbox();
  void BranchBackCheckCurrentTextbox();
  void StartReplaceJumpFallAnim();
  void BranchBackReplaceJumpFallAnim();
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
  
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  
  // Don't run if currently reloading the current screen
  if (ReloadCurrentScreen)
  {
    // If reloading the room with a Curse chest, make sure the chest is closed if the player has the Curse from it
    if (ttyd::string::strcmp(NextMap, "dou_07") == 0)
    {
      // Check if the Curse chest is open or not
      // Check if GSWF(2980) is off
      if (!(ttyd::swdrv::swGet(2980)))
      {
        // Manually close the chest if the player already has the curse
        if (ttyd::mario_pouch::pouchCheckItem(BoatModeCurse) > 0)
        {
          // Turn on GSWF(2980) to manually close the chest
          ttyd::swdrv::swSet(2980);
        
          // Update the Sequence to what it would normally be after getting the curse
          ttyd::swdrv::swByteSet(0, 247);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_06") == 0)
    {
      // Check if the Curse chest is open or not
      // Check if GSWF(1493) is off
      if (!(ttyd::swdrv::swGet(1493)))
      {
        // Manually close the chest if the player already has the curse
        if (ttyd::mario_pouch::pouchCheckItem(PaperModeCurse) > 0)
        {
          // Turn on GSWF(1493) to manually close the chest
          ttyd::swdrv::swSet(1493);
        
          // Update the Sequence to what it would normally be after getting the curse
          ttyd::swdrv::swByteSet(0, 45);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tik_19") == 0)
    {
      // Check if the Curse chest is open or not
      // Check if GSWF(1352) is off
      if (!(ttyd::swdrv::swGet(1352)))
      {
        // Manually close the chest if the player already has the curse
        if (ttyd::mario_pouch::pouchCheckItem(PlaneModeCurse) > 0)
        {
          // Turn on GSWF(1352) to manually close the chest
          ttyd::swdrv::swSet(1352);
        
          // Update the Sequence to what it would normally be after getting the curse
          ttyd::swdrv::swByteSet(0, 15);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "usu_01") == 0)
    {
      // Check if the Curse chest is open or not
      // Check if GSWF(1931) is off
      if (!(ttyd::swdrv::swGet(1931)))
      {
        // Manually close the chest if the player already has the curse
        if (ttyd::mario_pouch::pouchCheckItem(TubeModeCurse) > 0)
        {
          // Turn on GSWF(1931) to manually close the chest
          ttyd::swdrv::swSet(1931);
        
          // Update the Sequence to what it would normally be after getting the curse
          ttyd::swdrv::swByteSet(0, 187);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "mri_09") == 0)
    {
      // Check if the Blue Key chest is open or not
      // Check if GSWF(2852) is off
      if (!(ttyd::swdrv::swGet(2852)))
      {
        // Manually close the chest if the player already has the Blue Key
        if (ttyd::mario_pouch::pouchCheckItem(BlueKey) > 0)
        {
          // Turn on GSWF(2852) to manually close the chest
          ttyd::swdrv::swSet(2852);
          
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
    else if (ttyd::string::strcmp(NextMap, "tou_08") == 0)
    {
      // Turn off GSWF(2388) to clear any currently-registered fights
      ttyd::swdrv::swClear(2388);
    }
    else if (ttyd::string::strcmp(NextMap, "tou_10") == 0)
    {
      // Turn off GSWF(2388) to clear any currently-registered fights
      ttyd::swdrv::swClear(2388);
    }
    
    return;
  }
  
  // Don't run on the title screen
  if (ttyd::string::strcmp(NextMap, "title") == 0)
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
  
  // Warp back to Grodus' room if the player just beat him
  if (ttyd::string::strcmp(NextBero, "koopa_evt") == 0)
  {
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
  
  bool ConfirmNewMap = false;
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
      // Check if GSWF(4194) is off
      if (!(ttyd::swdrv::swGet(4194)))
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
    else if (ttyd::string::strcmp(NextMap, "aji_19") == 0)
    {
      // Change the loading zone to prevent spawning out of bounds
      ttyd::string::strcpy(NextBero, "w_bero");
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
    else if (ttyd::string::strcmp(NextMap, "dou_05") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "w_bero_2");
    }
    else if (ttyd::string::strcmp(NextMap, "dou_06") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "dou_12") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "w_bero_2");
    }
    else if (ttyd::string::strcmp(NextMap, "dou_13") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "e_bero_1");
    }
    else if (ttyd::string::strcmp(NextMap, "eki_02") == 0)
    {
      // Change the loading zone to prevent spawning out of bounds
      ttyd::string::strcpy(NextBero, "s_bero");
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
      // Change the loading zone used if the player has not opened the Ultra Boots chest yet
      // Check if GSWF(3728) is off
      if (!(ttyd::swdrv::swGet(3728)))
      {
        // Run if the 16 bit is off
        // Change the Sequence to allow the player to get the Ultra Boots
        if (SequencePosition > 318)
        {
          ttyd::swdrv::swByteSet(0, 318);
        }
        
        // Set the correct loading zone for the player to be able to get the Ultra Boots
        ttyd::string::strcpy(NextBero, "w_bero_1");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "eki_06") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "e_bero_2");
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
    else if (ttyd::string::strcmp(NextMap, "gon_02") == 0)
    {
      // Change the loading zone to prevent spawning close to a Koopa
      ttyd::string::strcpy(NextBero, "e_bero");
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
          ttyd::swdrv::swClear(1493);
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
          ttyd::swdrv::swClear(1494);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gon_08") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "e_bero_1");
    }
    else if (ttyd::string::strcmp(NextMap, "gon_09") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "e_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "gon_10") == 0)
    {
      // Change the loading zone to prevent spawning out of bounds
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "gon_11") == 0)
    {
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 56
      if ((SequencePosition < 56) && CheckChallengeModeTimerCutoff())
      {
        continue;
      }
      else
      {
        // Change the loading zone to prevent spawning under the map
        ttyd::string::strcpy(NextBero, "w_bero");
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
    else if (ttyd::string::strcmp(NextMap, "gor_02") == 0)
    {
      // Reset GSWF(1207) to allow the player to get the follower again
      ttyd::swdrv::swClear(1207);
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
      if (SequencePosition < 188)
      {
        // Only change if the Sequence is currently before the value set by blowing away the tree
        // Change the loading zone to prevent the player from being forced to update the Sequence to leave the room
        ttyd::string::strcpy(NextBero, "w_bero_1");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "gra_03") == 0)
    {
      // Change the loading zone to prevent falling through the floor
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "gra_06") == 0)
    {
      // Change the loading zone to prevent falling through the floor
      ttyd::string::strcpy(NextBero, "n_bero");
      
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
    else if (ttyd::string::strcmp(NextMap, "hei_06") == 0)
    {
      // Change the loading zone to prevent falling through the floor
      ttyd::string::strcpy(NextBero, "e_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "hei_08") == 0)
    {
      // Change the loading zone to prevent spawning on the stone or enemy
      ttyd::string::strcpy(NextBero, "e_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "hei_10") == 0)
    {
      if (CheckChallengeModeTimerCutoff())
      {
        if (SequencePosition <= 31)
        {
          // Check if the player has gotten the Sun Stone
          if (ttyd::swdrv::swGet(1774))
          {
            // Check if the player has gotten the Moon Stone
            if (ttyd::swdrv::swGet(1775))
            {
              // Prevent being able to fight the Gold Fuzzy if the Sequence is set for it and the player has both stones
              continue;
            }
          }
        }
      }
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
        else if (SequencePosition < 198)
        {
          // Check if the player has the Steeple Key
          if (ttyd::mario_pouch::pouchCheckItem(SteepleKey1) == 0)
          {
            // Change the loading zone to allow the player to get the Steeple Key
            ttyd::string::strcpy(NextBero, "e_bero_3");
            
            // Adjust the Sequence to skip the intro cutscene if necessary
            if (SequencePosition < 194)
            {
              // Set the Sequence to 194 to prevent the cutscene from playing
              ttyd::swdrv::swByteSet(0, 194);
            }
          }
          else
          {
            // Set the Sequence to 198 to prevent the player from getting another Steeple Key
            ttyd::swdrv::swByteSet(0, 198);
          }
        }
      }
      else if (SequencePosition < 198)
      {
        // Check if the player has the Steeple Key
        if (ttyd::mario_pouch::pouchCheckItem(SteepleKey1) == 0)
        {
          // Change the loading zone to allow the player to get the Steeple Key
          ttyd::string::strcpy(NextBero, "e_bero_3");
          
          if (LZRandoChallenge && (SequencePosition < 194))
          {
            // Adjust the Sequence to skip the intro cutscene if necessary
            // Set the Sequence to 194 to prevent the cutscene from playing
            ttyd::swdrv::swByteSet(0, 194);
          }
        }
        else
        {
          // Set the Sequence to 198 to prevent the player from getting another Steeple Key
          ttyd::swdrv::swByteSet(0, 198);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "jin_04") == 0)
    { 
      // Check if the sequence is before or at Doopliss 2
      if (SequencePosition <= 210)
      {
        // Check if the Sequence is before or at Doopliss 1
        if (SequencePosition <= 199)
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          if (CheckChallengeModeTimerCutoff())
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
          if (CheckChallengeModeTimerCutoff())
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
      ttyd::swdrv::swByteSet(1321, NewPitFloor); // GSW(1321)
      
      // Reset NextMap to proper Pit map
      ttyd::string::strcpy(NextMap, reinterpret_cast<char *>(NewPitMap));
      
      // Change loading zone to the pipe above the room
      ttyd::string::strcpy(NextBero, "dokan_2");
    }
    else if (ttyd::string::strcmp(NextMap, "las_01") == 0)
    {
      // Change the loading zone to prevent falling through the floor
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "las_02") == 0)
    {
      // Change the loading zone to prevent spawning on the Bill Blasters
      ttyd::string::strcpy(NextBero, "w_bero");
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
    else if (ttyd::string::strcmp(NextMap, "las_06") == 0)
    {
      // Change the loading zone to prevent spawning on the Bill Blasters
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
    else if (ttyd::string::strcmp(NextMap, "las_21") == 0)
    {
      if (SequencePosition < 390)
      {
        // The room changes once the Sequence reaches 390
        // Change the loading zone to prevent spawning on the Chain Chomp
        ttyd::string::strcpy(NextBero, "w_bero");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_23") == 0)
    {
      if (SequencePosition < 390)
      {
        // The room changes once the Sequence reaches 390
        // Change the loading zone to prevent spawning over spikes
        ttyd::string::strcpy(NextBero, "w_bero");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_24") == 0)
    {
      // Change the loading zone to prevent spawning near the Chain Chomp
      ttyd::string::strcpy(NextBero, "s_bero_2");
    }
    else if (ttyd::string::strcmp(NextMap, "las_25") == 0)
    {
      if (SequencePosition < 390)
      {
        // The room changes once the Sequence reaches 390
        // Change the loading zone to prevent spawning near the Phantom Embers
        ttyd::string::strcpy(NextBero, "e_bero");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_26") == 0)
    {
      if (SequencePosition <= 387)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          if (SequencePosition == 387)
          {
            // Prevent the player from being able to fight Gloomtail if the Sequence is exactly 387
            continue;
          }
        }
        else
        {
          // Allow Gloomtail to be fought if the Sequence is before or at 387
          // Set the Sequence to 387 so that Gloomtail can be fought
          ttyd::swdrv::swByteSet(0, 387);
        }
      }
      else if (SequencePosition == 400)
      {
        // Prevent warping to this room if the Sequence is 400, or else the Shadow Queen cutscene will occur
        continue;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "las_28") == 0)
    {
      if (SequencePosition <= 399)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow Grodus and Bowser & Kammy to be fought if the Sequence is before or at 399
          // Set the Sequence to 399 so that Grodus and Bowser & Kammy can be fought
          ttyd::swdrv::swByteSet(0, 399);
          ttyd::string::strcpy(NextBero, "w_bero");
        }
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
    else if (ttyd::string::strcmp(NextMap, "las_30") == 0)
    {
      // Change the loading zone to prevent spawning under the map
      ttyd::string::strcpy(NextBero, "w_bero");
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
    else if (ttyd::string::strcmp(NextMap, "moo_02") == 0)
    {
      // Change the loading zone to prevent spawning on the Yux
      ttyd::string::strcpy(NextBero, "w_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "mri_00") == 0)
    {
      // Reset GSWF(2826) to allow the player to get the follower again
      ttyd::swdrv::swClear(2826);
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
    else if (ttyd::string::strcmp(NextMap, "mri_12") == 0)
    {
      // Change the loading zone to prevent spawning next to enemies
      ttyd::string::strcpy(NextBero, "e_bero");
    }
    else if (ttyd::string::strcmp(NextMap, "muj_00") == 0)
    {
      if (SequencePosition <= 259)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          if (SequencePosition == 259)
          {
            // Prevent the player from being able to fight Crump if the Sequence is exactly 259
            continue;
          }
          else
          {
            // Change the loading zone to prevent landing at the middle of the water
            ttyd::string::strcpy(NextBero, "n_bero_02");
          }
        }
        else
        {
          // Allow Crump to be fought if the Sequence is before or at 259
          // Set the Sequence to 259 so that Crump can be fought
          ttyd::swdrv::swByteSet(0, 259);
        }
      }
      else
      {
        // Change the loading zone to prevent landing at the middle of the water
        ttyd::string::strcpy(NextBero, "n_bero_02");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "muj_03") == 0)
    {
      // Change the loading zone to prevent falling through the floor
      ttyd::string::strcpy(NextBero, "e_bero");
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
    else if (ttyd::string::strcmp(NextMap, "pik_01") == 0)
    {
      // Prevent warping to this room if the Sequence is set for the Bowser section
      // Only prevent if currently using the challenge mode
      if (LZRandoChallenge)
      {
        if ((SequencePosition == 373) || (SequencePosition == 374))
        {
          // Get a new map
          continue;
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
      if (SequencePosition < 332)
      {
        // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 332
        if (CheckChallengeModeTimerCutoff())
        {
          continue;
        }
      }
      else
      {
        // Change the loading zone to prevent spawning under the map
        ttyd::string::strcpy(NextBero, "w_bero");
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tik_02") == 0)
    {
      if (SequencePosition <= 20)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          if (SequencePosition == 20)
          {
            // Prevent the player from being able to fight Blooper if the Sequence is exactly 20
            continue;
          }
        }
        else
        {
          // Allow Blooper to be fought if the Sequence is before or at 20
          // Set the Sequence to 20 so that Blooper can be fought
          ttyd::swdrv::swByteSet(0, 20);
        }
      }
    }
    else if (ttyd::string::strcmp(NextMap, "tik_04") == 0)
    {
      // Change the loading zone to prevent spawning close to enemies
      ttyd::string::strcpy(NextBero, "n_bero_5");
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
    else if (ttyd::string::strcmp(NextMap, "tik_20") == 0)
    {
      // Change the loading zone to prevent spawning near the Spanias
      ttyd::string::strcpy(NextBero, "w_bero");
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
          
          // Turn off GSWF(2388) to clear any currently-registered fights
          ttyd::swdrv::swClear(2388);
        }
      }
      else
      {
        // Turn off GSWF(2388) to clear any currently-registered fights
        ttyd::swdrv::swClear(2388);
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
    else if (ttyd::string::strcmp(NextMap, "tou_08") == 0)
    {
      // Turn off GSWF(2388) to clear any currently-registered fights
      ttyd::swdrv::swClear(2388);
    }
    else if (ttyd::string::strcmp(NextMap, "tou_10") == 0)
    {
      // Turn off GSWF(2388) to clear any currently-registered fights
      ttyd::swdrv::swClear(2388);
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
    else if (ttyd::string::strcmp(NextMap, "usu_01") == 0)
    {
      // Check if the player has the Curse or not
      if (ttyd::mario_pouch::pouchCheckItem(TubeModeCurse) == 0)
      {
        // Check if the player has the Black Key for the chest
        if (ttyd::mario_pouch::pouchCheckItem(BlackKey3) > 0)
        {
          // Turn off GSWF(1931) to manually open the chest
          ttyd::swdrv::swClear(1931);
          
          // Manually set the Sequence to allow the player to get the Curse if they have the key for it
          ttyd::swdrv::swByteSet(0, 185);
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
  MarioFreeze = false;
  
  // Set up stuff for new file
  // Set Sequence to be just past getting Goombella
  ttyd::swdrv::swByteSet(0, 7);
  
  // Add all partners
  for (int i = 1; i <= 7; i++)
  {
    ttyd::mario_party::partyJoin(static_cast<ttyd::party::PartyMembers>(i));
  }
  
  // Reset party stats to what they normally would be
  ttyd::mario_pouch::pouchRevisePartyParam();
  
  // Heal all party members, as they will not be healed by pouchRevisePartyParam
  ttyd::evt_pouch::evt_pouch_all_party_recovery();
  
  // Randomize Yoshi color
  ttyd::party::yoshiSetColor();
  
  // Start with Yoshi out
  ttyd::mario_party::marioPartyHello(ttyd::party::PartyMembers::Yoshi);
  
  // Start with the Strange Sack
  ttyd::mario_pouch::pouchGetItem(StrangeSack);
  
  // Randomize Power Rush and Power Rush P icons
  // Can only be randomized in the US and EU versions
  #ifndef TTYD_JP
    #ifdef TTYD_US
      uint32_t itemDataTable = 0x803108A8;
    #elif defined TTYD_EU
      uint32_t itemDataTable = 0x8031C638;
    #endif
    
    // Randomly pick one of the 5 icons to use
    *reinterpret_cast<uint16_t *>(itemDataTable + (PowerRush * 0x28) + 0x20) = ttyd::system::irand(5) + 73;
    *reinterpret_cast<uint16_t *>(itemDataTable + (PowerRushP * 0x28) + 0x20) = ttyd::system::irand(5) + 103;
  #endif
  
  // Reset the flag used for Jump Storage
  ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
  player->flags3 &= ~((1 << 16) | (1 << 17)); // Turn off the 16 and 17 bits
  
  // Reset the lottery
  uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
  uint32_t LotteryAddress = GSWAddresses + 0xE0;
  *reinterpret_cast<uint32_t *>(LotteryAddress) = 0;
  *reinterpret_cast<uint32_t *>(LotteryAddress + 0x4) = 0;
  
  // Turn off a bit to enable loading zones
  *reinterpret_cast<uint32_t *>(GSWAddresses) &= ~(1 << 0); // Turn off the 0 bit
  
  // Turn on GSWF(0) to skip shop tutorials
  ttyd::swdrv::swSet(0);
  
  // Turn on GSWF(37) and GSWF(38) to prevent emails from being explained for the first time
  ttyd::swdrv::swSet(37);
  ttyd::swdrv::swSet(38);
  
  // Turn on GSWF(233) to skip save blocks from being explained for the first time
  ttyd::swdrv::swSet(233);
  
  // Turn on GSWF(234) to skip recovery blocks from being explained for the first time
  ttyd::swdrv::swSet(234);
  
  // Turn on GSWF(235) to skip items from being explained for the first time
  ttyd::swdrv::swSet(235);
  
  // Turn on GSWF(1187) to set Zess T. to be blocking the west entrance
  ttyd::swdrv::swSet(1187);
  
  // Turn on GSWF(1189) to have the Contact Lens ordered already
  ttyd::swdrv::swSet(1189);
  
  // Turn on GSWF(1197) to skip Zess T. explaining that he will cook stuff for the player now
  ttyd::swdrv::swSet(1197);
  
  // Turn on GSWF(1200) to prevent partners from explaining Save Blocks in central Rogueport
  ttyd::swdrv::swSet(1200);
  
  // Turn on GSWF(1334) to have the entrances revealed already in tik_03
  ttyd::swdrv::swSet(1334);
  
  // Turn on GSWF(1353) to skip having to talk to the Plane Mode curse chest for the first time
  ttyd::swdrv::swSet(1353);
  
  // Turn on GSWF(1492) to skip having to talk to the Paper Mode curse chest for the first time
  ttyd::swdrv::swSet(1492);
  
  // Turn on GSWF(1781) and GSWF(1782) to skip the Koopie Koo cutscene in Petal Meadows
  ttyd::swdrv::swSet(1781);
  ttyd::swdrv::swSet(1782);
  
  // Turn on GSWF(1805) to skip the cutscene of Goombella explaining her tattles on the bridge screen in Petal Meadows
  ttyd::swdrv::swSet(1805);
  
  // Turn on GSWF(1932) to skip having to talk to the Tube Mode curse chest for the first time
  ttyd::swdrv::swSet(1932);
  
  // Turn on GSWF(2075) to skip Vivian's textbox in Twilight Trail
  ttyd::swdrv::swSet(2075);
  
  // Turn on GSWF(2228) to prevent the player from being able to use the Steeple Key
  ttyd::swdrv::swSet(2228);
  
  // Turn on GSWF(2401) to skip the cutscene of entering Grubba's office through the grate for the first time
  ttyd::swdrv::swSet(2401);
  
  // Turn on GSWF(2500) to skip the cutscene of re-signing up to be a fighter
  ttyd::swdrv::swSet(2500);
  
  // Turn on GSWF(2867) to drain the water in the Great Tree
  ttyd::swdrv::swSet(2867);
  
  // Turn on GSWF(2878) to prevent the player from being able to talk to Jabble
  ttyd::swdrv::swSet(2878);
  
  // Turn on GSWF(2982), GSWF(2983), and GSWF(2984) to activate the blue switches in Pirate's Grotto
  ttyd::swdrv::swSet(2982);
  ttyd::swdrv::swSet(2983);
  ttyd::swdrv::swSet(2984);
  
  // Turn on GSWF(3131) to skip the cutscene with Four-Eyes after Bobbery is taken by the Embers
  ttyd::swdrv::swSet(3131);
  
  // Turn on GSWF(3574) to skip the cutscene of the bridge being shown when talking to the conductor at Riverside
  ttyd::swdrv::swSet(3574);
  
  // Turn on GSWF(3884) to skip the cutscene on the first screen of the area leading to Fahr Outpost
  ttyd::swdrv::swSet(3884);
  
  // Turn on GSWF(4196) and GSWF(4197) to allow the player to use the crane without needing to insert the Cog
  ttyd::swdrv::swSet(4196);
  ttyd::swdrv::swSet(4197);
  
  // Turn on GSWF(4218) to skip the crane game tutorial
  ttyd::swdrv::swSet(4218);
  
  // Turn on GSWF(5374) to skip the Trouble Center tutorial
  ttyd::swdrv::swSet(5374);
}

void overwriteNewFileStrings()
{
  // Overwrite aaa_00 and prologue strings
  #ifdef TTYD_US
    uint32_t aaa_00_Address = 0x802EDE78;
  #elif defined TTYD_JP
    uint32_t aaa_00_Address = 0x802ED930;
  #elif defined TTYD_EU
    uint32_t aaa_00_Address = 0x802F9AD8;
  #endif
  uint32_t prologue_Address = aaa_00_Address + 0x8;

  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Load = ttyd::seqdrv::SeqIndex::kLoad;
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  ttyd::seqdrv::SeqIndex Title = ttyd::seqdrv::SeqIndex::kTitle;
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  
  // Only change the strings on the load screen
  if (NextSeq == Load)
  {
    if (seqMainCheck == 2)
    {
      if (LZRando)
      {
        // Set the strings for the Loading Zone randomizer
        ttyd::string::strcpy(reinterpret_cast<char *>(aaa_00_Address), "gor_01");
        ttyd::string::strcpy(reinterpret_cast<char *>(prologue_Address), "s_bero");
      }
      else
      {
        // Set the strings to default
        ttyd::string::strcpy(reinterpret_cast<char *>(aaa_00_Address), "aaa_00");
        ttyd::string::strcpy(reinterpret_cast<char *>(prologue_Address), "prologue");
      }
    }
    else if (seqMainCheck == 4)
    {
      // Only clear the cache once
      if (!ClearCacheFlag)
      {
        ClearCacheFlag = true;
        ttyd::OSCache::DCFlushRange(reinterpret_cast<void *>(aaa_00_Address), 16);
      }
    }
  }
  else if (ClearCacheFlag && ((NextSeq == Game) || (NextSeq == Title)))
  {
    // Reset the flag so that the cache can be cleared later when needed
    ClearCacheFlag = false;
  }
}

void marioNeverTransform()
{
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  
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
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  
  if (NextSeq != Game)
  {
    return;
  }
  
  // Reset SQWarpAway flag, as it needs to be off once in a new room
  SQWarpAway = false;
  
  // Reset JustDefeatedBoss, so that it can be used for other bosses
  JustDefeatedBoss = 0;
  
  // Clear chapter end head pictures if the next loading zone is a pipe
  if (ttyd::string::strncmp(NextBero, "dokan", 5) == 0)
  {
    uint32_t fbatModeAddress = reinterpret_cast<uint32_t>(ttyd::npcdrv::fbatGetPointer());
    int16_t fbatMode = *reinterpret_cast<int16_t *>(fbatModeAddress);
    
    // Only run if the fbat mode is set to 1, otherwise it will conflict with other code
    if (fbatMode == 1)
    {
      // Get the proper pointer to pass into imgRelease
      uint32_t HeadImageAddress = *reinterpret_cast<uint32_t *>(imgEntryAddress);
      HeadImageAddress = *reinterpret_cast<uint32_t *>(HeadImageAddress + 0x4);
      
      // imgRelease automatically searches for and clears any images within the specified area
      ttyd::imgdrv::imgRelease(reinterpret_cast<void *>(HeadImageAddress));
    }
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
      
      if ((CurrentPartnerOut == 0) || (CurrentPartnerOut > static_cast<uint8_t>(ttyd::party::PartyMembers::Koops)))
      {
        // Bring out Goombella if either no partner is out or a partner from past Chapter 1 is out
        ttyd::mario_party::marioPartyEntry(ttyd::party::PartyMembers::Goombella);
      }
    }
  }
  if (ttyd::string::strcmp(NextMap, "gor_01") == 0)
  {
    if (MarioFreeze)
    {
      uint32_t RopeAddress = *reinterpret_cast<uint32_t *>(_mapEntAddress);
      RopeAddress= *reinterpret_cast<uint32_t *>(RopeAddress + 0x154);
      float RopePosZ = *reinterpret_cast<float *>(RopeAddress + 0x8B4);
      
      ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
      
      player->flags1 |= (1 << 1); // Turn on the 1 bit
      player->wJumpVelocityY = 0;
      player->wJumpAccelerationY = 0;
      player->unk_84 = 0;
      player->playerPosition[0] = -1;
      player->playerPosition[1] = 59;
      player->playerPosition[2] = RopePosZ + 10;
      
      #ifdef TTYD_JP
        player->unk_19c = 180;
        player->wPlayerDirection = 180;
      #else
        player->unk_1a0 = 180;
        player->unk_1b0 = 180;
      #endif
    }
  }
  else if (ttyd::string::strcmp(NextMap, "jin_04") == 0)
  {
    if (SequencePosition == 200)
    {
      // Warp to a different room to skip the cutscene of Doopliss getting the Ruby star
      ttyd::swdrv::swByteSet(0, 201);
      ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "jin_04", "w_bero");
    }
    else if ((SequencePosition == 210) || (SequencePosition == 211))
    {
      // Check if the Sequence is currently set to fight Doopliss 2
      uint8_t CurrentPartnerOut = getPartnerOut(PartnerPointer);
      
      if (CurrentPartnerOut != static_cast<uint8_t>(ttyd::party::PartyMembers::Vivian))
      {
        // Force Vivian out
        ttyd::mario_party::marioPartyEntry(ttyd::party::PartyMembers::Vivian);
      }
    }
  }
  else if (ttyd::string::strcmp(NextMap, "las_29") == 0)
  {
    // Make sure a partner is out for the Shadow Queen cutscene
    if (!PartnerPointer)
    {
      // Bring out Goombella if no partner is out
      ttyd::mario_party::marioPartyHello(ttyd::party::PartyMembers::Goombella);
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
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  
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
    player->flags2 &= ~((1 << 2) | (1 << 12)); // Turn off the 2 and 12 bits
    player->wAnimPosition[1] = 0;
    player->wModelPosition[0] = 0;
    player->wModelPosition[2] = 0;
  }
}

void resetMarioThroughLZ()
{
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex MapChange = ttyd::seqdrv::SeqIndex::kMapChange;
  
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
  
  ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
  uint16_t ReceivingItem = 15;
  
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  ttyd::seqdrv::SeqIndex MapChange = ttyd::seqdrv::SeqIndex::kMapChange;
  
  if ((ButtonInput & ReloadScreenCombo) == ReloadScreenCombo)
  {
    if ((NextSeq < Game) || (NextSeq > MapChange))
    {
      return;
    }
    
    ttyd::seqdrv::SeqIndex Seq = ttyd::seqdrv::seqGetSeq();
    ttyd::seqdrv::SeqIndex Battle = ttyd::seqdrv::SeqIndex::kBattle;
    
    if (Seq == Battle)
    {
      // Reloading the room with the Seq set to Battle will cause the game to crash, so don't allow it
      return;
    }
    
    // Prevent reloading the room if currently getting an item. This prevents the player from reloading the room after opening a chest in order to respawn it closed, while at the same time keeping the item from it.
    
    if ((player->currentMotionId == ReceivingItem) && (NextSeq != MapChange))
    {
      // Only allow if not currently reloading the room already
      return;
    }
    
    // Prevent reloading the room if the player is currently in a room with a boss. This prevents the player from repeatedly killing the boss for SP and/or points.
    uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
    if (ttyd::string::strcmp(NextMap, "aji_14") == 0)
    {
      if (SequencePosition <= 373)
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
    else if (ttyd::string::strcmp(NextMap, "hei_10") == 0)
    {
      if (SequencePosition <= 31)
      {
        // Check if the player has gotten the Sun Stone
        if (ttyd::swdrv::swGet(1774))
        {
          // Check if the player has gotten the Moon Stone
          if (ttyd::swdrv::swGet(1775))
          {
            return;
          }
        }
      }
    }
    else if (ttyd::string::strncmp(NextMap, "jon_00", 3) == 0)
    {
      // Prevent the player from constantly reloading floors to kill high level enemies
      return;
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
    else if (ttyd::string::strcmp(NextMap, "las_28") == 0)
    {
      if (SequencePosition == 399)
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
    else if (ttyd::string::strcmp(NextMap, "mri_01") == 0)
    {
      if ((SequencePosition == 110) || (SequencePosition == 111))
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(NextMap, "muj_00") == 0)
    {
      if (SequencePosition == 259)
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
    MarioFreeze = false;
    
    // Reset the black screen fade effect set when loading into a room via a pipe
    uint32_t fadedrvAddress = *reinterpret_cast<uint32_t *>(wp_fadedrv_Address);
    *reinterpret_cast<uint16_t *>(fadedrvAddress) &= ~(1 << 15); // Turn off the 15 bit
    
    // Clear chapter end head pictures if any are currently present
    uint32_t HeadImageAddress = *reinterpret_cast<uint32_t *>(imgEntryAddress);
    HeadImageAddress = *reinterpret_cast<uint32_t *>(HeadImageAddress + 0x4);
    ttyd::imgdrv::imgRelease(reinterpret_cast<void *>(HeadImageAddress));
    
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
    
    // Reset the camera - mainly for the black bars at the top and bottom of the screen
    uint32_t CameraPointer = reinterpret_cast<uint32_t>(ttyd::camdrv::camGetPtr(8));
    *reinterpret_cast<uint16_t *>(CameraPointer) &= ~(1 << 9); // Turn off the 9 bit
    
    return;
  }
  
  // Manually reset the System Level and the camera if currently getting an item while the room is trying to load
  if ((player->currentMotionId == ReceivingItem) && (NextSeq == MapChange))
  {
    // Reset the System Level
    ttyd::mariost::marioStSystemLevel(0);
    
    // Reset the camera - mainly for the black bars at the top and bottom of the screen
    uint32_t CameraPointer = reinterpret_cast<uint32_t>(ttyd::camdrv::camGetPtr(8));
    *reinterpret_cast<uint16_t *>(CameraPointer) &= ~(1 << 9); // Turn off the 9 bit
  }
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
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex GameOver = ttyd::seqdrv::SeqIndex::kGameOver;
    
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
  
  ReloadCurrentScreen = false;
  MarioFreeze = false;
  GameOverFlag = true;
}

void reloadCurrentScreenFlag()
{
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  
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

extern "C" {
char *checkCurrentTextbox(char *currentText)
{
  // Only set flags if using the challenge mode
  if (LZRandoChallenge)
  {
    if (ttyd::string::strcmp(currentText, "mac_0_126") == 0)
    {
      // Just defeated Blooper
      JustDefeatedBoss = 1;
    }
    else if (ttyd::string::strcmp(currentText, "stg1_hei_44") == 0)
    {
      // Just defeated the Gold Fuzzy
      JustDefeatedBoss = 2;
    }
    else if (ttyd::string::strcmp(currentText, "stg1_gon_33_03") == 0)
    {
      // Just defeated Hooktail
      JustDefeatedBoss = 3;
    }
    else if (ttyd::string::strcmp(currentText, "stg2_mri_e25_00_00") == 0)
    {
      // Just defeated Magnus 1.0
      JustDefeatedBoss = 4;
    }
    else if (ttyd::string::strcmp(currentText, "stg3_tou_473") == 0)
    {
      // Just defeated Grubba
      JustDefeatedBoss = 5;
    }
    else if (ttyd::string::strcmp(currentText, "stg5_dou_25") == 0)
    {
      // Just defeated Cortez
      JustDefeatedBoss = 6;
    }
    else if (ttyd::string::strcmp(currentText, "stg6_rsh_247") == 0)
    {
      // Just defeated Smorg
      JustDefeatedBoss = 7;
    }
    else if (ttyd::string::strcmp(currentText, "stg7_aji_42") == 0)
    {
      // Just defeated Magnus 2.0
      JustDefeatedBoss = 8;
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_24") == 0)
    {
      // Just defeated the Shadow Sirens (Chapter 8)
      JustDefeatedBoss = 9;
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_70") == 0)
    {
      // Just defeated Grodus
      JustDefeatedBoss = 10;
      
      // Skip most of the Grodus dialogue by warping to the Bowser part
      ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "las_28", "koopa_evt");
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_101") == 0)
    {
      // Just defeated Bowser & Kammy
      JustDefeatedBoss = 11;
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_139") == 0)
    {
      // Set a flag to warp away from the Shadow Queen fight if the player says No
      SQWarpAway = true;
    }
  }
  
  return currentText;
}
}

extern "C" {
const char *replaceJumpFallAnim(char *jumpFallString)
{
  if (ttyd::string::strncmp(jumpFallString, "M_J_", 4) == 0)
  {
    if (ttyd::string::strcmp(NextMap, "gor_01") == 0)
    {
      // Return an arbitrary string
      const char *newString = "w_bero";
      
      if (MarioFreeze)
      {
        return newString;
      }
      else
      {
        ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
      
        // Should use evt_sub_check_intersect instead
        if ((player->playerPosition[0] >= -10) && (player->playerPosition[0] <= 10))
        {
          if ((player->playerPosition[2] >= 250) && (player->playerPosition[2] <= 267))
          {
            if (player->playerPosition[1] >= 58)
            {
              MarioFreeze = true;
              return newString;
            }
          }
        }
      }
    }
  }
  
  return jumpFallString;
}
}

void Mod::preventPartyLeft(ttyd::party::PartyMembers id)
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

int32_t Mod::randomizeGivenFollower(ttyd::party::PartyMembers id, float x, float y, float z)
{
  // Randomize given followers
  // Only randomize if the Loading Zone randomizer is currently in use
  if (LZRando)
  {
    id = static_cast<ttyd::party::PartyMembers>(ttyd::system::irand(5) + 8);
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

uint32_t Mod::warpAwayFromSQ(void *ptr)
{
  if (SQWarpAway)
  {
    // Prevent getting a Game Over if the player said No to the Shadow Queen
    // Warp to a different room
    ttyd::swdrv::swByteSet(0, 405);
    ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "las_29", "w_bero");
    
    // Prevent the function from running, so that it does not trigger a Game Over
    return 0;
  }
  else
  {
    // Call original function
    return mPFN_warpAwayFromSQ_trampoline(ptr);
  }
}

bool Mod::preventGetItemOnReload(uint32_t id)
{
  // Only prevent from running if currently reloading a room and if the Loading Zone randomizer is currently in use
  if (ReloadCurrentScreen && LZRando)
  {
    return false;
  }
  else
  {
    // Call original function
    return mPFN_preventGetItemOnReload_trampoline(id);
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
    uint32_t CheckCurrentTextbox = 0x800D2880;
    uint32_t PowerRushIconCheck = 0x8001A320;
    uint32_t PowerRushPIconCheck = 0x8001A314;
    uint32_t ReplaceJumpFallAnim = 0x800411D0;
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
    uint32_t CheckCurrentTextbox = 0x800CE750;
    uint32_t ReplaceJumpFallAnim = 0x80040B34;
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
    uint32_t CheckCurrentTextbox = 0x800D3678;
    uint32_t PowerRushIconCheck = 0x8001A4E4;
    uint32_t PowerRushPIconCheck = 0x8001A4D8;
    uint32_t ReplaceJumpFallAnim = 0x800412B8;
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
  
  // Check the current textbox to check for specific things to happen
  patch::writeBranch(reinterpret_cast<void *>(CheckCurrentTextbox), reinterpret_cast<void *>(StartCheckCurrentTextbox));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackCheckCurrentTextbox), reinterpret_cast<void *>(CheckCurrentTextbox + 0x4));
  
  // Allow the Power Rush and Power Rush P icons to be set to the JP icon
  // Does not need to be changed for the JP version
  #ifndef TTYD_JP
    *reinterpret_cast<uint32_t *>(PowerRushIconCheck) = 0x60000000; // nop
    *reinterpret_cast<uint32_t *>(PowerRushPIconCheck) = 0x60000000; // nop
  #endif
  
  patch::writeBranch(reinterpret_cast<void *>(ReplaceJumpFallAnim), reinterpret_cast<void *>(StartReplaceJumpFallAnim));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackReplaceJumpFallAnim), reinterpret_cast<void *>(ReplaceJumpFallAnim + 0x4));
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
}

}