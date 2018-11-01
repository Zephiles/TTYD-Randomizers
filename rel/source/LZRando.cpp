#include "mod.h"
#include "maps.h"
#include "items.h"
#include "buttons.h"
#include "patch.h"

#include <ttyd/swdrv.h>
#include <ttyd/string.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/pmario_sound.h>
#include <ttyd/system.h>
#include <ttyd/evtmgr.h>
#include <ttyd/npcdrv.h>
#include <ttyd/mot_ship.h>
#include <ttyd/fadedrv.h>
#include <ttyd/mario_party.h>
#include <ttyd/evt_pouch.h>
#include <ttyd/mario.h>
#include <ttyd/seqdrv.h>
#include <ttyd/OSCache.h>
#include <ttyd/camdrv.h>
#include <ttyd/mariost.h>
#include <ttyd/mario_cam.h>

#include <cstdio>

extern uint16_t Rawk_Hawk_GSWF_Array_Size;
extern uint16_t Rawk_Hawk_GSWF_Array[];
extern char *NextBero;
extern bool ChangedLZ;
extern char *NextMap;
extern bool LZRandoChallenge;
extern uint32_t TimerCount;
extern bool LZRando;
extern char *NextArea;
extern bool ReloadCurrentScreen;
extern bool GameOverFlag;
extern bool NewFile;
extern bool GameOverChallengeMode;
extern uint32_t PossibleChallengeMaps[];
extern uint16_t ChallengeMapArraySize;
extern uint32_t PossibleLZMaps[];
extern uint16_t LZMapArraySize;
extern bool DefeatedRawkHawk;
extern uint32_t GSWAddressesStart;
extern bool TransformIntoShip;
extern bool CloseCurtainFlag;
extern bool MarioFreeze;
extern uint16_t NewFile_GSWF_Array_Size;
extern uint16_t NewFile_GSWF_Array[];
extern uint32_t seqMainAddress;
extern bool ClearCacheFlag;
extern uint8_t SQWarpAwayCounter;
extern uint8_t JustDefeatedBoss;
// extern uint32_t AreaObjectsAddressesStart;
// extern uint32_t AreaLZsAddressesStart;
// extern uint32_t NPCAddressesStart;
extern uint32_t _mapEntAddress;
extern char *NewBero;
extern char *NewMap;
extern uint32_t wp_fadedrv_Address;
extern bool ResetSystemFlag;

extern "C" {
  void StartGetRandomWarp();
  void BranchBackGetRandomWarp();
  void StartEnablePaperTubeModes();
  void StartEnableBoatMode();
  void BranchBackEnableBoatMode();
  void StartEnablePlaneMode();
  void BranchBackEnablePlaneMode();
  void StartMarioKeyOnFix();
  void BranchBackMarioKeyOnFix();
  void StartEnableWalkJumpOnWater();
  void StartEnableLandOnWater();
  void BranchBackEnableLandOnWater();
  void StartSpawnPartnerBattle();
  void BranchBackSpawnPartnerBattle();
  void StartPreventBattleOnRespawn();
  void BranchBackPreventBattleOnRespawn();
  void StartCheckCurrentTextbox();
  void BranchBackCheckCurrentTextbox();
  void StartReplaceJumpFallAnim();
  void BranchBackReplaceJumpFallAnim();
  void StartRandomizeBeroOnFileLoad();
  void BranchBackRandomizeBeroOnFileLoad();
  void StartResetSystemFlag();
  void BranchBackResetSystemFlag();
  void StartAdjustTitleScreenTimer();
  void BranchBackAdjustTitleScreenTimer();
}

namespace mod {

void InitRawkHawk()
{
  // Turn off specific GSWFs
  uint16_t ArraySize = Rawk_Hawk_GSWF_Array_Size; // Prevent the compiled code from loading this variable more than once
  for (int i = 0; i < ArraySize; i++)
  {
    ttyd::swdrv::swClear(Rawk_Hawk_GSWF_Array[i]);
  }
  
  
  for (int i = 0; i < 4; i++)
  {
    // Turn on GSWF(2513) through GSWF(2516) to skip Rawk Hawk's in-battle dialogye
    ttyd::swdrv::swSet(2513 + i);
    
    // Turn on GSW(501) to make sure that Rawk Hawk is the current battle
    // Turn on GSW(503) and GSW(504) to make sure that nothing goes wrong when setting up the fight
    // Don't set GSW(502)
    if (i != 1)
    {
      ttyd::swdrv::swByteSet(501 + i, 0);
    }
  }
  
  // Turn on GSW(502) so that the battle condtion is to use a special move at least once
  ttyd::swdrv::swByteSet(502, 6);
  
  // Turn on GSWF(2388) to make sure that a match is registered
  ttyd::swdrv::swSet(2388);
  
  // Turn on GSWF(2509) to make sure that the Goomba first strike has occured already
  ttyd::swdrv::swSet(2509);
}

void CheckMapForReloadChanges()
{
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  char *tempNextMap = NextMap; // Prevent NextMap from being loaded in multiple times
  
  // If reloading the room with a Curse chest, make sure the chest is closed if the player has the Curse from it
  if (ttyd::string::strcmp(tempNextMap, "dou_07") == 0)
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
        
        // Turn off the curse background music
        ttyd::pmario_sound::psndBGMOff(0x201);
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "gon_06") == 0)
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
        
        // Turn off the curse background music
        ttyd::pmario_sound::psndBGMOff(0x201);
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "tik_19") == 0)
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
        
        // Turn off the curse background music
        ttyd::pmario_sound::psndBGMOff(0x201);
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "usu_01") == 0)
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
        
        // Turn off the curse background music
        ttyd::pmario_sound::psndBGMOff(0x201);
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "hom_00") == 0)
  {
    // Check if the player is currently able to obtain the Station Key
    if (SequencePosition == 310)
    {
      // Check if the player has gotten the Station Key already
      if (ttyd::mario_pouch::pouchCheckItem(StationKey1) > 0)
      {
        // Update the Sequence to what it would normally be after getting the Station Key
        ttyd::swdrv::swByteSet(0, 311);
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "mri_09") == 0)
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
  else if (ttyd::string::strcmp(tempNextMap, "tou_01") == 0)
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
  else if (ttyd::string::strcmp(tempNextMap, "tou_08") == 0)
  {
    // Turn off GSWF(2388) to clear any currently-registered fights
    ttyd::swdrv::swClear(2388);
    
    // Turn off GSWF(2389) to prevent winning coins/going down a rank
    ttyd::swdrv::swClear(2389);
  }
  else if (ttyd::string::strcmp(tempNextMap, "tou_10") == 0)
  {
    // Turn off GSWF(2388) to clear any currently-registered fights
    ttyd::swdrv::swClear(2388);
    
    // Turn off GSWF(2389) to prevent winning coins/going down a rank
    ttyd::swdrv::swClear(2389);
  }
}

bool CheckChallengeModeTimerCutoff()
{
  if (!LZRandoChallenge)
  {
    return false;
  }
  
  uint32_t TimerCountCutoff = ttyd::system::sysMsec2Frame(2400000); // 40 minutes
  if (TimerCount >= TimerCountCutoff)
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
  
  char *tempNextMap = NextMap; // Prevent NextMap from being loaded in multiple times
  char *tempNextBero = NextBero; // Prevent NextBero from being loaded in multiple times
  char *tempNextArea = NextArea; // Prevent NextArea from being loaded in multiple times
  
  // Don't run if currently reloading the current screen
  if (ReloadCurrentScreen)
  {
    if (!ChangedLZ)
    {
      // Write a temporary string into NextBero to tell the player that the new loading zone hasn't been chosen yet
      ttyd::string::strcpy(tempNextBero, "Pending");
    }
    
    CheckMapForReloadChanges();
    return;
  }
  
  // Don't run on the title screen
  if (ttyd::string::strcmp(tempNextMap, "title") == 0)
  {
    return;
  }
  
  // Don't run if transitioning to the intro
  // However, it should still run under these conditions if the Game Over flag is set
  if (ttyd::string::strcmp(tempNextMap, "dmo_00") == 0)
  {
    if (!GameOverFlag)
    {
      return;
    }
  }
  
  // Warp back to Magnus 2.0's room if the player just beat him
  if (ttyd::string::strcmp(tempNextBero, "evt_aji_14") == 0)
  {
    ttyd::string::strcpy(tempNextBero, "tuzuki");
    ttyd::string::strcpy(tempNextMap, "aji_14");
    ttyd::string::strncpy(tempNextArea, "aji_14", 3);
    
    // Prevent the loading zone from being changed
    ChangedLZ = true;
    return;
  }
  
  // Warp back to Grodus' room if the player just beat him
  if (ttyd::string::strcmp(tempNextBero, "koopa_evt") == 0)
  {
    return;
  }
  
  // Warp to the second fight of the Shadow Queen if the player just beat the first fight
  if (ttyd::string::strcmp(tempNextBero, "minnnanokoe") == 0)
  {
    return;
  }
  
  // Don't run on the credits screen
  if (ttyd::string::strcmp(tempNextMap, "end_00") == 0)
  {
    return;
  }
  
  // Write a temporary string into NextBero to tell the player that the new loading zone hasn't been chosen yet
  ttyd::string::strcpy(tempNextBero, "Pending");
  
  ChangedLZ = false;
  
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  
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
    GameOverChallengeMode = false;
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
    ttyd::string::strcpy(tempNextMap, NewRandomMap);
    ttyd::string::strncpy(tempNextArea, NewRandomMap, 3);
    
    if (ttyd::string::strcmp(tempNextMap, "aji_00") == 0)
    {
      // Skip the cutscenes and fight
      // Need to skip this cutscene and fight to prevent a crash
      if (SequencePosition < 361)
      {
        // Set the Sequence to 361 to prevent the cutscene from playing and to prevent the fight from occuring
        ttyd::swdrv::swByteSet(0, 361);
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "aji_07") == 0)
    {
      // Set the Loading Zone to be able to get the Cog if the player doesn't have it already
      // Check if GSWF(4194) is off
      if (!(ttyd::swdrv::swGet(4194)))
      {
        ttyd::string::strcpy(tempNextBero, "tenjo");
        
        // Prevent the loading zone from being changed
        ChangedLZ = true;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "aji_08") == 0)
    {
      // Force the loading zone to s_bero_1, as the other possible loading zones are invalid
      ttyd::string::strcpy(tempNextBero, "s_bero_1");
        
      // Prevent the loading zone from being changed
      ChangedLZ = true;
    }
    else if (ttyd::string::strcmp(tempNextMap, "aji_13") == 0)
    {
      // Manually turn off GW(11) to prevent the game from crashing because of the conveyor belt
      uint32_t EvtWork = reinterpret_cast<uint32_t>(ttyd::evtmgr::evtGetWork());
      *reinterpret_cast<uint32_t *>(EvtWork + 0x30) = 0;
    }
    else if (ttyd::string::strcmp(tempNextMap, "aji_14") == 0)
    {
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 373
      if ((SequencePosition < 373) && CheckChallengeModeTimerCutoff())
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "dou_07") == 0)
    {
      // Only check if the player hasn't gotten the Boat Mode curse yet
      if (!(ttyd::swdrv::swGet(2980)))
      {
        if (SequencePosition != 246)
        {
          // Automaticallt adjust the Sequence if the player talked to the chest but didn't fight the Embers
          // Automatically adjust the Sequence if the Embers were defeated but the player did not get the key
          // Automatically adjust the Sequence if the player got the Black Key but didn't open the chest
          // if (ttyd::swdrv::swGet(2988) || ttyd::swdrv::swGet(2989) || ttyd::swdrv::swGet(2987))
          if (ttyd::swdrv::swGet(2988)) // Should only need to check 2988, as the others cannot be on if 2988 isn't
          {
            // Set the Sequence to 246 to be able to open the chest
            ttyd::swdrv::swByteSet(0, 246);
          }
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "eki_05") == 0)
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
        ttyd::string::strcpy(tempNextBero, "w_bero_1");
        
        // Prevent the loading zone from being changed
        ChangedLZ = true;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "gon_01") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "gon_03") == 0)
    {
      if (SequencePosition <= 38)
      {
        // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
        if (CheckChallengeModeTimerCutoff())
        {
          continue;
        }
        else
        {
          if (LZRandoChallenge)
          {
            // Only set the Sequence when using the challenge mode, as the cutscene will also set it
            ttyd::swdrv::swByteSet(0, 38);
          }
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "gon_06") == 0)
    {
      // Adjust the Sequence and GSWF(1493) if the player hasn't gotten the Paper Curse yet
      if (ttyd::mario_pouch::pouchCheckItem(PaperModeCurse) == 0)
      {
        // Only run if the player has the Black Key
        if (ttyd::mario_pouch::pouchCheckItem(BlackKey2) > 0)
        {
          // Adjust the Sequence if necessary, as the Sequence being past 44 will turn on GSWF(1493)
          if (SequencePosition > 44)
          {
            ttyd::swdrv::swByteSet(0, 44);
          }
          
          // Turn off GSWF(1493)
          ttyd::swdrv::swClear(1493);
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "gon_07") == 0)
    {
      // Adjust the Sequence and GSWF(1494) if the player hasn't gotten the Paper Curse yet
      if (ttyd::mario_pouch::pouchCheckItem(PaperModeCurse) == 0)
      {
        // Only run if the player hasn't gotten the Black Key yet
        if (ttyd::mario_pouch::pouchCheckItem(BlackKey2) == 0)
        {
          // Adjust the Sequence if necessary, as the Sequence being past 42 will Turn on GSWF(1494)
          if (SequencePosition > 42)
          {
            ttyd::swdrv::swByteSet(0, 42);
          }
          
          // Turn off GSWF(1494)
          ttyd::swdrv::swClear(1494);
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "gon_11") == 0)
    {
      // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 56
      if ((SequencePosition < 56) && CheckChallengeModeTimerCutoff())
      {
        continue;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "gon_12") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "gor_02") == 0)
    {
      // Turn off GSWF(1207) to allow the player to get the follower again
      ttyd::swdrv::swClear(1207);
    }
    else if (ttyd::string::strcmp(tempNextMap, "gra_06") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "hei_00") == 0)
    {
      if (LZRandoChallenge)
      {
        // Skip the intro cutscene and the Hooktail flying cutscene if using the challenge mode
        if (SequencePosition < 24)
        {
          // Set the Sequence to 24 to prevent the cutscene from playing
          ttyd::swdrv::swByteSet(0, 24);
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "hei_08") == 0)
    {
      // Manually check to see if the player has the Moon Stone or not
      if (ttyd::mario_pouch::pouchCheckItem(MoonStone) == 0)
      {
        // The player doesn't have the Moon Stone, so make sure it can spawn
        // Turn off GSWF(1775) so that the Moon Stone can spawn
        ttyd::swdrv::swClear(1775);
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "hei_10") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "hei_12") == 0)
    {
      // Manually check to see if the player has the Sun Stone or not
      if (ttyd::mario_pouch::pouchCheckItem(SunStone) == 0)
      {
        // The player doesn't have the Sun Stone, so make sure it can spawn
        // Turn off GSWF(1774) so that the Sun Stone can spawn
        ttyd::swdrv::swClear(1774);
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "hom_00") == 0)
    {
      if (LZRandoChallenge)
      {
        // Skip the cutscene with Doopliss if using the challenge mode
        if (SequencePosition <= 310)
        {
          // Check if the player has gotten the Station Key
          if (ttyd::mario_pouch::pouchCheckItem(StationKey1) > 0)
          {
            // Adjust the Sequence to prevent the player from getting multiple Station Keys
            ttyd::swdrv::swByteSet(0, 311);
          }
          else
          {
            // Set the Sequence to 310 to prevent the cutscene from playing
            ttyd::swdrv::swByteSet(0, 310);
          }
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "jin_00") == 0)
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
            // Only change the loading zone if using the challenge mode
            if (LZRandoChallenge)
            {
              ttyd::string::strcpy(tempNextBero, "e_bero_3");
              
              // Prevent the loading zone from being changed
              ChangedLZ = true;
            }
            
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
          // Only change the loading zone if using the challenge mode
          if (LZRandoChallenge)
          {
            ttyd::string::strcpy(tempNextBero, "e_bero_3");
            
            // Prevent the loading zone from being changed
            ChangedLZ = true;
          }
          
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
    else if (ttyd::string::strcmp(tempNextMap, "jin_04") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "jon_00") == 0)
    {
      // Prevent warping to the Pit if 10 minutes have not passed
      uint32_t TimerCountCutoff = ttyd::system::sysMsec2Frame(3000000); // 50 minutes
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
      ttyd::string::strcpy(tempNextMap, reinterpret_cast<char *>(NewPitMap));
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_05") == 0)
    {
      if (SequencePosition <= 382)
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else
        {
          // Allow the Dark Bones to be fought if the Sequence is before or at 382
          // Set the Sequence to 382 so that the Dark Bones can be fought
          ttyd::swdrv::swByteSet(0, 382);
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_09") == 0)
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
          ttyd::string::strcpy(tempNextBero, "e_bero");
          
          // Prevent the loading zone from being changed
          ChangedLZ = true;
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_26") == 0)
    {
      // Force the loading zone to w_bero, since s_bero is a possible random choice but is invalid
      ttyd::string::strcpy(tempNextBero, "w_bero");
      
      // Prevent the loading zone from being changed
      ChangedLZ = true;
      
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
    else if (ttyd::string::strcmp(tempNextMap, "las_28") == 0)
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
          ttyd::string::strcpy(tempNextBero, "w_bero");
          
          // Prevent the loading zone from being changed
          ChangedLZ = true;
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_29") == 0)
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
          ttyd::string::strcpy(tempNextBero, "sekai_yami2");
          
          // Prevent the loading zone from being changed
          ChangedLZ = true;
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "moo_00") == 0)
    {
      // Skip the intro cutscene
      // Need to skip this cutscene to prevent a crash
      if (SequencePosition < 358)
      {
        // Set the Sequence to 358 to prevent the cutscene from playing
        ttyd::swdrv::swByteSet(0, 358);
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "mri_00") == 0)
    {
      // Turn off GSWF(2826) to allow the player to get the follower again
      ttyd::swdrv::swClear(2826);
    }
    else if (ttyd::string::strcmp(tempNextMap, "mri_01") == 0)
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
          
          // Manually set the loading zone to prevent the game from crashing
          ttyd::string::strcpy(tempNextBero, "e_bero");
          
          // Prevent the loading zone from being changed
          ChangedLZ = true;
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "mri_03") == 0)
    {
      // Skip the Dried Shroom cutscene with Petuni if using the challenge mode
      if (LZRandoChallenge && (SequencePosition == 98))
      {
        // Set the Sequence to 100 to prevent the cutscene from playing
        ttyd::swdrv::swByteSet(0, 100);
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "muj_00") == 0)
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
        }
        else
        {
          // Allow Crump to be fought if the Sequence is before or at 259
          // Set the Sequence to 259 so that Crump can be fought
          ttyd::swdrv::swByteSet(0, 259);
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "muj_12") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "nok_00") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "pik_01") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "rsh_05_a") == 0)
    {
      // The game will crash if the player enters this room with the Sequence being greater than 338
      if (SequencePosition > 338)
      {
        // Get a new map
        continue;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "rsh_06_a") == 0)
    {
      if (SequencePosition < 332)
      {
        // Get a new map if currently using the challenge mode, 20 minutes have not passed, and the Sequence is less than 332
        if (CheckChallengeModeTimerCutoff())
        {
          continue;
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "tik_02") == 0)
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
          ttyd::string::strcpy(tempNextBero, "w_bero_1");
          
          // Prevent the loading zone from being changed
          ChangedLZ = true;
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "tik_03") == 0)
    {
      // Turn on GSWF(1334) to have the entrances revealed already
      ttyd::swdrv::swSet(1334);
    }
    else if (ttyd::string::strcmp(tempNextMap, "tik_05") == 0)
    {
      if (SequencePosition == 15)
      {
        // Set the Sequence to 16 to prevent the cutscene from playing
        ttyd::swdrv::swByteSet(0, 16);
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "tou_01") == 0)
    {
      // Adjust the Sequence to allow the player to get the Super Hammer if they don't have it already
      if (ttyd::mario_pouch::pouchCheckItem(SuperHammer) == 0)
      {
        // Set the Sequence to 139 so that the player can get the Super Hammer
        ttyd::swdrv::swByteSet(0, 139);
      }
      else if (SequencePosition == 139)
      {
        // Set the Sequence to 140 so that the player can't get the Super Hammer again
        ttyd::swdrv::swByteSet(0, 140);
      }
      else if (LZRandoChallenge)
      {
        if (SequencePosition < 127)
        {
          // Set the Sequence to 127 to prevent the intro cutscene from playing
          ttyd::swdrv::swByteSet(0, 127);
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "tou_03") == 0)
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
          if (SequencePosition == 163)
          {
            // Only Grubba can be fought when the Sequence is exactly 163
            ttyd::string::strcpy(tempNextBero, "w_bero");
          
            // Prevent the loading zone from being changed
            ChangedLZ = true;
            
            // Turn off GSWF(2388) to clear any currently-registered fights
            ttyd::swdrv::swClear(2388);
          }
          else
          {
            // Allow Rawk Hawk or Grubba to be fought at random
            bool BossCheck = ttyd::system::irand(1000) < 500;
            
            if (BossCheck)
            {
              // Set up to fight Rawk Hawk
              if (SequencePosition < 133)
              {
                // Set the Sequence past 128 to prevent the Rawk Hawk and Koopinator cutscene from playing
                ttyd::swdrv::swByteSet(0, 133);
              }
              
              InitRawkHawk();
              ttyd::string::strcpy(tempNextBero, "w_bero");
                  
              // Prevent the loading zone from being changed
              ChangedLZ = true;
            }
            else
            {
              // Set up to fight Grubba
              // Set the Sequence to 163 so that Grubba can be fought
              ttyd::swdrv::swByteSet(0, 163);
              ttyd::string::strcpy(tempNextBero, "w_bero");
            
              // Prevent the loading zone from being changed
              ChangedLZ = true;
              
              // Turn off GSWF(2388) to clear any currently-registered fights
              ttyd::swdrv::swClear(2388);
            }
          }
        }
      }
      else
      {
        if (CheckChallengeModeTimerCutoff())
        {
          // Get a new map if currently using the challenge mode and 20 minutes have not passed yet
          continue;
        }
        else if (LZRandoChallenge)
        {
          // Force the Rawk Hawk fight if using the challenge mode
          // Set up to fight Rawk Hawk
          InitRawkHawk();
          ttyd::string::strcpy(tempNextBero, "w_bero");
              
          // Prevent the loading zone from being changed
          ChangedLZ = true;
        }
        else
        {
          // Randomly choose if Rawk Hawk should be fought or not
          bool RawkHawkCheck = ttyd::system::irand(1000) < 500;
          
          if (RawkHawkCheck)
          {
            // Set up to fight Rawk Hawk
            InitRawkHawk();
            ttyd::string::strcpy(tempNextBero, "w_bero");
                
            // Prevent the loading zone from being changed
            ChangedLZ = true;
          }
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "tou_04") == 0)
    {
      // Turn off GSWF(2384) and GSWF(2385) to prevent the pipe cutscenes from playing
      ttyd::swdrv::swClear(2384);
      ttyd::swdrv::swClear(2385);
    }
    else if (ttyd::string::strcmp(tempNextMap, "tou_05") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "tou_06") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "tou_07") == 0)
    {
      if (DefeatedRawkHawk)
      {
        DefeatedRawkHawk = false;
        
        // Turn on GSWF(2389) and turn off GSWF(2503) to allow the player to get the Champ's Belt
        ttyd::swdrv::swSet(2389);
        ttyd::swdrv::swClear(2503);
        
        // Adjust the Sequence to allow the player to get the Champ's Belt
        if (SequencePosition < 172)
        {
          // Set the Sequence to 172 to allow the player to get the Champ's Belt
          ttyd::swdrv::swByteSet(0, 172);
        }
        
        // Lock the loading zone to North to save time
        ttyd::string::strcpy(tempNextBero, "n_bero");
          
        // Prevent the loading zone from being changed
        ChangedLZ = true;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "tou_08") == 0)
    {
      // Prevent warping to this room if using the challenge mode and the Sequence is less than 130
      if (LZRandoChallenge && (SequencePosition < 130))
      {
        // Warp to a different room, as the Grubba intro cutscene is currently set to play
        continue;
      }
      
      // Turn off GSWF(2388) to clear any currently-registered fights
      ttyd::swdrv::swClear(2388);
      
      // Turn off GSWF(2389) to prevent winning coins/going down a rank
      ttyd::swdrv::swClear(2389);
    }
    else if (ttyd::string::strcmp(tempNextMap, "tou_10") == 0)
    {
      // Turn off GSWF(2388) to clear any currently-registered fights
      ttyd::swdrv::swClear(2388);
      
      // Turn off GSWF(2389) to prevent winning coins/going down a rank
      ttyd::swdrv::swClear(2389);
    }
    else if (ttyd::string::strcmp(tempNextMap, "usu_00") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "usu_01") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "win_00") == 0)
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
          ttyd::string::strcpy(tempNextBero, "w_bero");
          
          // Prevent the loading zone from being changed
          ChangedLZ = true;
        }
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "win_01") == 0)
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

bool CheckLoadingZones(const char *CurrentBero, const char *LoadingZones[], uint32_t ArraySize)
{
  for (uint32_t i = 0; i < ArraySize; i++)
  {
    if (ttyd::string::strcmp(CurrentBero, LoadingZones[i]) == 0)
    {
      return true;
    }
  }
  
  return false;
}

bool CheckIfValidLoadingZone(const char *CurrentBero)
{
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  char *tempNextMap = NextMap; // Prevent NextMap from being loaded in multiple times
  
  if (ttyd::string::strcmp(tempNextMap, "aji_13") == 0)
  {
    const char *LoadingZones[] = {
      "dokan_6",
      "dokan_7",
      "dokan_8",
      "dokan_9",
      "dokan_10" };
    uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
    
    if (CheckLoadingZones(CurrentBero, LoadingZones, LoadingZoneArraySize))
    {
      return false;
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "gor_03") == 0)
  {
    const char *LoadingZones[] = {
      "s_bero",
      "A_dokan_02" };
    uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
    
    if (CheckLoadingZones(CurrentBero, LoadingZones, LoadingZoneArraySize))
    {
      return false;
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "gra_04") == 0)
  {
    const char *LoadingZones[] = {
      "w_bero_1",
      "w_bero_2" };
    uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
    
    if (CheckLoadingZones(CurrentBero, LoadingZones, LoadingZoneArraySize))
    {
      return false;
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "gra_05") == 0)
  {
    const char *LoadingZones[] = {
      "ana_1",
      "ana_2",
      "A_ana_1",
      "A_ana_2" };
    uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
    
    if (CheckLoadingZones(CurrentBero, LoadingZones, LoadingZoneArraySize))
    {
      return false;
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "las_19") == 0)
  {
    if (SequencePosition < 390)
    {
      // Lower part of the room is not loaded
      const char *LoadingZones[] = {
        "n_bero",
        "e_bero_2" };
      uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
      
      if (CheckLoadingZones(CurrentBero, LoadingZones, LoadingZoneArraySize))
      {
        return false;
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "las_21") == 0)
  {
    if (SequencePosition < 390)
    {
      // Lower part of the room is not loaded
      const char *LoadingZones[] = {
        "n_bero",
        "e_bero_2" };
      uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
      
      if (CheckLoadingZones(CurrentBero, LoadingZones, LoadingZoneArraySize))
      {
        return false;
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "las_23") == 0)
  {
    if (SequencePosition < 390)
    {
      // Lower part of the room is not loaded
      const char *LoadingZones[] = {
        "n_bero_2",
        "n_bero_1",
        "e_bero_2" };
      uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
      
      if (CheckLoadingZones(CurrentBero, LoadingZones, LoadingZoneArraySize))
      {
        return false;
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "las_25") == 0)
  {
    if (SequencePosition < 390)
    {
      // Lower part of the room is not loaded
      const char *LoadingZones[] = {
        "n_bero_1",
        "e_bero_2" };
      uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
      
      if (CheckLoadingZones(CurrentBero, LoadingZones, LoadingZoneArraySize))
      {
        return false;
      }
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "tik_00") == 0)
  {
    if (ttyd::string::strcmp(CurrentBero, "tensou") == 0)
    {
      return false;
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "win_02") == 0)
  {
    if (ttyd::string::strcmp(CurrentBero, "dokan1") == 0)
    {
      return false;
    }
  }
  
  return true;
}

uint32_t Mod::getRandomLZ(void *scriptContext, uint32_t waitMode)
{
  // Only run if using the Loading Zone randomizer
  if (LZRando)
  {
    char *tempNextBero = NextBero; // Prevent NextBero from being loaded in multiple times
    char *tempNextMap = NextMap; // Prevent NextMap from being loaded in multiple times
    
    // Only randomize if the loading zone was not manually changed
    if (!ChangedLZ)
    {
      // Get the start of the loading zone addresses
      uint32_t scriptContextLZAddresses = reinterpret_cast<uint32_t>(scriptContext) + 0x9C;
      scriptContextLZAddresses = *reinterpret_cast<uint32_t *>(scriptContextLZAddresses);
      
      // Loop through the loading zones and get the total amount of them
      uint32_t tempScriptContextAddress = scriptContextLZAddresses;
      uint32_t LoadingZoneTotal = 0;
      
      while (*reinterpret_cast<uint32_t *>(tempScriptContextAddress) != 0)
      {
        tempScriptContextAddress += 0x3C;
        LoadingZoneTotal++;
      }
      
      // Make sure at least one valid loading zone was found
      if (LoadingZoneTotal > 0)
      {
        // Select a random loading zone
        uint32_t LoadingZoneToUse;
        uint32_t tempNewLoadingZone;
        const char *NewLoadingZone;
        
        // Make sure the loading zone chosen is valid
        do
        {
          LoadingZoneToUse = ttyd::system::irand(LoadingZoneTotal);
          tempNewLoadingZone = *reinterpret_cast<uint32_t *>(scriptContextLZAddresses + (LoadingZoneToUse * 0x3C));
          NewLoadingZone = reinterpret_cast<const char *>(tempNewLoadingZone);
        }
        while (!CheckIfValidLoadingZone(NewLoadingZone));
        
        // Set the chosen loading zone
        uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
        char *NextBeroGSW = reinterpret_cast<char *>(GSWAddresses + 0x11C);
        ttyd::string::strcpy(NextBeroGSW, NewLoadingZone);
        ttyd::string::strcpy(tempNextBero, NewLoadingZone);
      }
    }
    
    // If not using the challenge mode, check for loading zones where Boat Mode should be used
    if (!LZRandoChallenge)
    {
      if (ttyd::string::strcmp(tempNextMap, "dou_02") == 0)
      {
        if (ttyd::string::strcmp(tempNextBero, "e_bero_1") == 0)
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "dou_03") == 0)
      {
        const char *LoadingZones[] = {
          "w_bero_1",
          "e_bero_2",
          "s_bero" };
        uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
        
        if (CheckLoadingZones(tempNextBero, LoadingZones, LoadingZoneArraySize))
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "dou_05") == 0)
      {
        if (ttyd::string::strcmp(tempNextBero, "w_bero_2") == 0)
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "dou_09") == 0)
      {
        const char *LoadingZones[] = {
          "e_bero_1",
          "n_bero" };
        uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
        
        if (CheckLoadingZones(tempNextBero, LoadingZones, LoadingZoneArraySize))
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "dou_10") == 0)
      {
        const char *LoadingZones[] = {
          "w_bero_1",
          "e_bero" };
        uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
        
        if (CheckLoadingZones(tempNextBero, LoadingZones, LoadingZoneArraySize))
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "dou_11") == 0)
      {
        if (ttyd::string::strcmp(tempNextBero, "w_bero") == 0)
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "dou_12") == 0)
      {
        const char *LoadingZones[] = {
          "w_bero_1",
          "e_bero_1" };
        uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
        
        if (CheckLoadingZones(tempNextBero, LoadingZones, LoadingZoneArraySize))
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "muj_00") == 0)
      {
        if (ttyd::string::strcmp(tempNextBero, "n_bero") == 0)
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "tik_15") == 0)
      {
        const char *LoadingZones[] = {
          "w_bero_1",
          "e_bero" };
        uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
        
        if (CheckLoadingZones(tempNextBero, LoadingZones, LoadingZoneArraySize))
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "tik_16") == 0)
      {
        const char *LoadingZones[] = {
          "w_bero_1",
          "e_bero_1" };
        uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
        
        if (CheckLoadingZones(tempNextBero, LoadingZones, LoadingZoneArraySize))
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "tik_17") == 0)
      {
        const char *LoadingZones[] = {
          "w_bero_1",
          "e_bero_1" };
        uint32_t LoadingZoneArraySize = sizeof(LoadingZones) / sizeof(LoadingZones[0]);
        
        if (CheckLoadingZones(tempNextBero, LoadingZones, LoadingZoneArraySize))
        {
          TransformIntoShip = true;
        }
      }
      else if (ttyd::string::strcmp(tempNextMap, "tik_18") == 0)
      {
        if (ttyd::string::strcmp(tempNextBero, "e_bero_1") == 0)
        {
          TransformIntoShip = true;
        }
      }
      
      // Check if Boat Mode should be used for the current loading zone
      if (TransformIntoShip)
      {
        // Force Mario to transform into the boat
        ttyd::mot_ship::marioReInit_ship();
      }
    }
    
    // Manually transform into the boat if the current room is tik_20, regardless of whether the challenge mode is being used or not
    if (ttyd::string::strcmp(tempNextMap, "tik_20") == 0)
    {
      ttyd::mot_ship::marioReInit_ship();
      
      // Prevent other functions from taking Mario out of Boat Mode
      TransformIntoShip = true;
    }
    
    // Check if the curtain needs to be cleared when loading a file
    if (CloseCurtainFlag)
    {
      CloseCurtainFlag = false;
      
      // Only close the curtain if coming out of a pipe
      if (ttyd::string::strncmp(tempNextBero, "dokan_6", 5) == 0)
      {
        // Only close the curtain if GSW(4) is not 21, as this is the value used when loading a file that has just saved after the credits
        uint32_t GSW_4 = ttyd::swdrv::swByteGet(4);
        if (GSW_4 != 21)
        {
          uint32_t color = 0x000000FF;
          int duration = 0;
          
          // Close the curtain
          const int CloseCurtain = 28;
          ttyd::fadedrv::fadeEntry(CloseCurtain, duration, reinterpret_cast<uint8_t *>(&color));
          
          // Set the screen to black
          const int BlackFadeOut = 10;
          ttyd::fadedrv::fadeEntry(BlackFadeOut, duration, reinterpret_cast<uint8_t *>(&color));
        }
      }
    }
  }
  
  // Call original function
  return mPFN_getRandomLZ_trampoline(scriptContext, waitMode);
}

void setUpNewFile()
{
  if (!NewFile)
  {
    return;
  }
  
  NewFile = false;
  MarioFreeze = false;
  
  // Set up stuff for new file
  // Set Sequence to be just past the Bandit stealing Mario's coins
  ttyd::swdrv::swByteSet(0, 8);
  
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
  
  // Turn off a bit to enable loading zones - probably not needed, but keeping as a failsafe
  *reinterpret_cast<uint32_t *>(GSWAddresses) &= ~(1 << 0); // Turn off the 0 bit
  
  // Turn on all of the GSWFs in the array
  uint16_t ArraySize = NewFile_GSWF_Array_Size; // Prevent the compiled code from loading this variable more than once
  for (int i = 0; i < ArraySize; i++)
  {
    ttyd::swdrv::swSet(NewFile_GSWF_Array[i]);
  }
}

void overwriteNewFileStrings()
{
  // Overwrite aaa_00 string
  #ifdef TTYD_US
    uint32_t aaa_00_Address = 0x802EDE78;
  #elif defined TTYD_JP
    uint32_t aaa_00_Address = 0x802ED930;
  #elif defined TTYD_EU
    uint32_t aaa_00_Address = 0x802F9AD8;
  #endif

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
        // Set the string for the Loading Zone randomizer
        ttyd::string::strcpy(reinterpret_cast<char *>(aaa_00_Address), "gor_01");
      }
      else
      {
        // Set the string to default
        ttyd::string::strcpy(reinterpret_cast<char *>(aaa_00_Address), "aaa_00");
      }
    }
    else if (seqMainCheck == 4)
    {
      // Only clear the cache once
      if (!ClearCacheFlag)
      {
        ClearCacheFlag = true;
        ttyd::OSCache::DCFlushRange(reinterpret_cast<void *>(aaa_00_Address), 6);
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
  ttyd::seqdrv::SeqIndex MapChange = ttyd::seqdrv::SeqIndex::kMapChange;
  
  char *tempNextMap = NextMap; // Prevent NextMap from being loaded in multiple times
  
  if (NextSeq == Game)
  {
    // Reset TransformIntoShip, so that it doesn't conflict with other loading zones
    TransformIntoShip = false;
    
    // Reset SQWarpAwayCounter to 0, as it needs to be off once in a new room
    SQWarpAwayCounter = 0;
    
    // Reset JustDefeatedBoss, so that it can be used for other bosses
    JustDefeatedBoss = 0;
    
    uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
    uint32_t PartnerPointer = reinterpret_cast<uint32_t>(ttyd::party::partyGetPtr(ttyd::mario_party::marioGetPartyId()));
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
    // uint32_t AreaObjectAddresses = *reinterpret_cast<uint32_t *>(AreaObjectsAddressesStart + 0x4);
    // uint32_t AreaLZAddresses = *reinterpret_cast<uint32_t *>(AreaLZsAddressesStart);
    // AreaLZAddresses = *reinterpret_cast<uint32_t *>(AreaLZAddresses + 0x4);
    
    if (ttyd::string::strcmp(tempNextMap, "gon_12") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "gor_01") == 0)
    {
      if (MarioFreeze)
      {
        uint32_t RopeAddress = *reinterpret_cast<uint32_t *>(_mapEntAddress);
        RopeAddress= *reinterpret_cast<uint32_t *>(RopeAddress + 0x154);
        float RopePosZ = *reinterpret_cast<float *>(RopeAddress + 0x8B4);
        
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
    else if (ttyd::string::strcmp(tempNextMap, "jin_04") == 0)
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
          // Check if Mario's Y Coordinate is 670 and that he is currently not moving
          if ((player->playerPosition[1] == 670) && (player->currentMotionId == 0))
          {
            // Force Vivian out
            ttyd::mario_party::marioPartyEntry(ttyd::party::PartyMembers::Vivian);
          }
        }
      }
      else if (SequencePosition == 213)
      {
        // Set the Sequence ahead to 214 to skip the Shadow Sirens cutscene in Twilight Town
        ttyd::swdrv::swByteSet(0, 214);
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_29") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "muj_00") == 0)
    {
      // Make sure a partner is out for the ship cutscene
      if (SequencePosition == 259)
      {
        if (!PartnerPointer)
        {
          // Bring out Goombella if no partner is out
          ttyd::mario_party::marioPartyHello(ttyd::party::PartyMembers::Goombella);
        }
      }
    }
  }
  else if (NextSeq == MapChange)
  {
    // Check for specific edits to REL files
    #ifdef TTYD_US
      const uint32_t eki_REL_ID = 0x6;
      const uint32_t jin_REL_ID = 0xD;
      uint32_t jin_party_kill_script_offset = 0x15108;
      uint32_t eki04_init_script_offset = 0xE5B8;
      uint32_t jin06_init_script_offset = 0x12100;
    #elif defined TTYD_JP
      const uint32_t eki_REL_ID = 0x7;
      const uint32_t jin_REL_ID = 0xE;
      uint32_t jin_party_kill_script_offset = 0x14FD8;
      uint32_t eki04_init_script_offset = 0xE5A8;
      uint32_t jin06_init_script_offset = 0x11FD0;
    #elif defined TTYD_EU
      const uint32_t eki_REL_ID = 0x7;
      const uint32_t jin_REL_ID = 0xE;
      uint32_t jin_party_kill_script_offset = 0x15108;
      uint32_t eki04_init_script_offset = 0xE5B8;
      uint32_t jin06_init_script_offset = 0x12100;
    #endif
    
    uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
    uint32_t REL_Pointer = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x15C);
    
    // Make sure a REL file is actually loaded
    if (REL_Pointer != 0)
    {
      uint32_t REL = *reinterpret_cast<uint32_t *>(REL_Pointer);
      switch (REL)
      {
        case eki_REL_ID:
        {
          // Make sure the current map is eki_04
          if (ttyd::string::strcmp(tempNextMap, "eki_04") == 0)
          {
            // Prevent eki_04 from adjusting the Sequence
            // Adjust the conditional to check for 0 instead of 316
            *reinterpret_cast<uint32_t *>(REL_Pointer + eki04_init_script_offset + 0xEC) = 0;
          }
          break;
        }
        case jin_REL_ID:
        {
          // Prevent partners and followers from being removed when in a jin map
          // Replace the first instruction with a return, as the entire function needs to be avoided
          *reinterpret_cast<uint32_t *>(REL_Pointer + jin_party_kill_script_offset) = 0x00000002; // return
          
          // Make sure the current map is jin_06
          if (ttyd::string::strcmp(tempNextMap, "jin_06") == 0)
          {
            // Prevent jin_06 from adjusting the Sequence
            // Adjust the conditional to check for 0 instead of 193, and replacing it with a return causes the game to get stuck in one of the evtEntry functions
            *reinterpret_cast<uint32_t *>(REL_Pointer + jin06_init_script_offset + 0x278) = 0;
          }
          break;
        }
        default:
        {
          break;
        }
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
  
  const uint16_t CurrentlyUsingYoshi = 26;
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
  
  const uint16_t CurrentlyUsingTubeMode = 22;
  const uint16_t CurrentlyUsingBoatMode = 25;
  const uint16_t CurrentlyUsingYoshi = 26;
  
  ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
  
  bool TubeModeComparison = player->currentMotionId == CurrentlyUsingTubeMode;
  bool BoatModeComparison = (player->currentMotionId == CurrentlyUsingBoatMode) && !TransformIntoShip;
  bool UsingYoshiComparison = player->currentMotionId == CurrentlyUsingYoshi;
  
  if (TubeModeComparison || BoatModeComparison || UsingYoshiComparison)
  {
    player->currentMotionId = 0;
  }
}

void reloadScreen()
{
  // Only run while using the challenge mode
  if (!LZRandoChallenge)
  {
    return;
  }
  
  uint32_t ButtonInput = ttyd::system::keyGetButton(0);
  uint16_t ReloadScreenCombo = PAD_L | PAD_B;
  
  ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
  const uint16_t ReceivingItem = 15;
  
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  ttyd::seqdrv::SeqIndex MapChange = ttyd::seqdrv::SeqIndex::kMapChange;
  
  uint32_t CameraPointer = reinterpret_cast<uint32_t>(ttyd::camdrv::camGetPtr(8));
  
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
    char *tempNextMap = NextMap; // Prevent NextMap from being loaded in multiple times
    
    if (ttyd::string::strcmp(tempNextMap, "aji_14") == 0)
    {
      if (SequencePosition <= 373)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "gon_11") == 0)
    {
      if (SequencePosition < 55)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "hei_10") == 0)
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
    else if (ttyd::string::strcmp(tempNextMap, "jin_00") == 0)
    {
      // Only check for the Sequences in which the Atomic Boo can spawn
      if (SequencePosition >= 197)
      {
        // If the player just got the Steeple Key, then the Sequence would be set to 198. They could then reload the room to fight the Atomic Boo, even if 20 minutes have not passed.
        if (CheckChallengeModeTimerCutoff() && (ttyd::mario_pouch::pouchGetHammerLv() > 1))
        {
          // The player is currently able to fight the Atomic Boo and 20 minutes have not passed, so don't allow reloading the room
          return;
        }
      }
    }
    else if (ttyd::string::strncmp(tempNextMap, "jon_00", 3) == 0)
    {
      // Prevent the player from constantly reloading floors to kill high level enemies
      return;
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_09") == 0)
    {
      if (SequencePosition == 390)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_26") == 0)
    {
      if (SequencePosition == 387)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_28") == 0)
    {
      if (SequencePosition == 399)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "las_29") == 0)
    {
      if (SequencePosition == 400)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "mri_01") == 0)
    {
      if ((SequencePosition == 110) || (SequencePosition == 111))
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "muj_00") == 0)
    {
      if (SequencePosition == 259)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "muj_12") == 0)
    {
      if (SequencePosition == 252)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "rsh_06_a") == 0)
    {
      if (SequencePosition < 332)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "tik_02") == 0)
    {
      if (SequencePosition == 20)
      {
        return;
      }
    }
    else if (ttyd::string::strcmp(tempNextMap, "tou_03") == 0)
    {
      if ((SequencePosition == 163) || (ttyd::swdrv::swGet(2388)))
      {
        return;
      }
    }
    
    // Not in the pause menu
    // A separate address for NextBero is needed, as the original value will be cleared during the reloading process
    // The game will crash if NextMap is used directly in seqSetSeq, so a separate address must be used instead
    // NewBero and NewMap need to be global variables
    
    ttyd::string::strcpy(NewBero, NextBero);
    ttyd::string::strcpy(NewMap, tempNextMap);
    ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, NewMap, NewBero);
    ReloadCurrentScreen = true;
    MarioFreeze = false;
    
    // Reset the black screen fade effect set when loading into a room via a pipe
    uint32_t fadedrvAddress = *reinterpret_cast<uint32_t *>(wp_fadedrv_Address);
    *reinterpret_cast<uint16_t *>(fadedrvAddress) &= ~(1 << 15); // Turn off the 15 bit
    
    // Reset the camera - mainly for the black bars at the top and bottom of the screen
    *reinterpret_cast<uint16_t *>(CameraPointer) &= ~((1 << 8) | (1 << 9)); // Turn off the 8 and 9 bits
    
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
    
    return;
  }
  
  // Manually reset the System Level and the camera if currently getting an item while the room is trying to load
  if ((player->currentMotionId == ReceivingItem) && (NextSeq == MapChange))
  {
    // Reset the System Level
    ttyd::mariost::marioStSystemLevel(0);
    
    // Reset the camera - mainly for the black bars at the top and bottom of the screen
    *reinterpret_cast<uint16_t *>(CameraPointer) &= ~((1 << 8) | (1 << 9)); // Turn off the 8 and 9 bits
  }
}

void setNextBero()
{
  // Only run while using the challenge mode
  if (!LZRandoChallenge)
  {
    return;
  }
  
  // Only run while a file is loaded
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  ttyd::seqdrv::SeqIndex Battle = ttyd::seqdrv::SeqIndex::kBattle;
  
  if ((NextSeq < Game) || (NextSeq > Battle))
  {
    return;
  }
  
  char *tempNextMap = NextMap; // Prevent NextMap from being loaded in multiple times
  
  // Prevent changing the loading zone if currently in a room where using it would result in a softlock
  if (ttyd::string::strcmp(tempNextMap, "eki_03") == 0)
  {
    return;
  }
  
  // Prevent changing the loading zone for rooms that need it to be set to a specific value
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  if (ttyd::string::strcmp(tempNextMap, "aji_14") == 0)
  {
    if (SequencePosition < 373)
    {
      return;
    }
  }
  else if (ttyd::string::strcmp(tempNextMap, "las_29") == 0)
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
  uint16_t CurrentBeroCombo = PAD_L | PAD_Y;
  
  char *tempNextBero = NextBero; // Prevent NextMap from being loaded in multiple times
  if ((ButtonInput & NBeroCombo) == NBeroCombo)
  {
    ttyd::string::strcpy(tempNextBero, "n_bero");
    
    // Prevent the loading zone from being changed
    ChangedLZ = true;
  }
  else if ((ButtonInput & SBeroCombo) == SBeroCombo)
  {
    ttyd::string::strcpy(tempNextBero, "s_bero");
    
    // Prevent the loading zone from being changed
    ChangedLZ = true;
  }
  else if ((ButtonInput & WBeroCombo) == WBeroCombo)
  {
    ttyd::string::strcpy(tempNextBero, "w_bero");
    
    // Prevent the loading zone from being changed
    ChangedLZ = true;
  }
  else if ((ButtonInput & EBeroCombo) == EBeroCombo)
  {
    ttyd::string::strcpy(tempNextBero, "e_bero");
    
    // Prevent the loading zone from being changed
    ChangedLZ = true;
  }
  else if ((ButtonInput & CurrentBeroCombo) == CurrentBeroCombo)
  {
    // Prevent the loading zone from being changed
    ChangedLZ = true;
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
  
  if (LZRandoChallenge)
  {
    GameOverChallengeMode = true;
  }
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

uint32_t getCurseReturnValue(uint32_t pouchCheckItem, const char *CheckMapArray[], uint32_t ArraySize)
{
  // Only run if the player doesn't already have the desired curse, and if the Loading Zone randomizer is in use
  if (!pouchCheckItem && LZRando)
  {
    char *tempNextMap = NextMap; // Prevent NextMap from being loaded in multiple times
    for (uint32_t i = 0; i < ArraySize; i++)
    {
      if (ttyd::string::strcmp(tempNextMap, CheckMapArray[i]) == 0)
      {
        // Return arbitrary value greater than 0
        return 1;
      }
    }
  }
  
  return pouchCheckItem;
}

extern "C" {
uint32_t enablePaperTubeModes(uint32_t pouchCheckItem)
{
  const char *CheckMapArray[] = {
    "aji_13",
    "usu_01",
    "jin_05",
    "jin_07",
    "jin_11",
    "eki_05" };
  uint32_t ArraySize = sizeof(CheckMapArray) / sizeof(CheckMapArray[0]);  
  
  return getCurseReturnValue(pouchCheckItem, CheckMapArray, ArraySize);
}
}

extern "C" {
uint32_t enableBoatMode(uint32_t pouchCheckItem)
{
  const char *CheckMapArray[] = {
    "tik_20",
    "dou_05",
    "dou_11" };
  uint32_t ArraySize = sizeof(CheckMapArray) / sizeof(CheckMapArray[0]);  
  
  return getCurseReturnValue(pouchCheckItem, CheckMapArray, ArraySize);
}
}

extern "C" {
uint32_t enablePlaneMode(uint32_t pouchCheckItem)
{
  const char *CheckMapArray[] = {
    "aji_13",
    "dou_11" };
  uint32_t ArraySize = sizeof(CheckMapArray) / sizeof(CheckMapArray[0]);  
  
  return getCurseReturnValue(pouchCheckItem, CheckMapArray, ArraySize);
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
  // Allow if the challenge mode is in use
  if (LZRandoChallenge)
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
    const uint32_t Goombella = 224;
    
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
  // Only run when using the challenge mode
  if (LZRandoChallenge)
  {
    // The reloadScreen function runs too late for this, so the button combo has to be checked
    uint32_t ButtonInput = ttyd::system::keyGetButton(0);
    uint16_t ReloadScreenCombo = PAD_L | PAD_B;
    
    if ((ButtonInput & ReloadScreenCombo) == ReloadScreenCombo)
    {
      return nullptr;
    }
  }
  
  return fbatPointer;
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
    else if ((ttyd::string::strcmp(currentText, "stg3_tou_438") == 0) || 
      (ttyd::string::strcmp(currentText, "stg3_tou_re_133") == 0))
    {
      // Just defeated Rawk Hawk
      JustDefeatedBoss = 5;
      
      // Set a flag to allow the player to get the Champ's Belt, even if the GSWF for it gets reset
      DefeatedRawkHawk = true;
    }
    else if (ttyd::string::strcmp(currentText, "stg3_tou_473") == 0)
    {
      // Just defeated Grubba
      JustDefeatedBoss = 6;
    }
    else if (ttyd::string::strcmp(currentText, "stg5_dou_25") == 0)
    {
      // Just defeated Cortez
      JustDefeatedBoss = 7;
    }
    else if (ttyd::string::strcmp(currentText, "stg6_rsh_247") == 0)
    {
      // Just defeated Smorg
      JustDefeatedBoss = 8;
    }
    else if (ttyd::string::strcmp(currentText, "stg7_aji_42") == 0)
    {
      // Just defeated Magnus 2.0
      JustDefeatedBoss = 9;
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_24") == 0)
    {
      // Just defeated the Shadow Sirens (Chapter 8)
      JustDefeatedBoss = 10;
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_70") == 0)
    {
      // Just defeated Grodus
      JustDefeatedBoss = 11;
      
      // Skip most of the Grodus dialogue by warping to the Bowser part
      ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "las_28", "koopa_evt");
      
      // Prevent the loading zone from being changed
      ChangedLZ = true;
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_101") == 0)
    {
      // Just defeated Bowser & Kammy
      JustDefeatedBoss = 12;
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_139") == 0)
    {
      // Set a value to warp away from the Shadow Queen fight if the player says No
      SQWarpAwayCounter = 1;
    }
    else if (ttyd::string::strcmp(currentText, "stg8_las_146") == 0)
    {
      // Skip the dialogue at the end of the first Shadow Queen fight
      ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "las_29", "minnnanokoe");
      
      // Prevent the loading zone from being changed
      ChangedLZ = true;
    }
  }
  
  return currentText;
}
}

extern "C" {
const char *replaceJumpFallAnim(char *jumpFallString)
{
  if (LZRandoChallenge)
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
  }
  
  return jumpFallString;
}
}

extern "C" {
uint32_t randomizeBeroOnFileLoad(uint32_t swByteGetCheck, void *GSWAddress, uint32_t NewSystemFlagValue)
{
  // Only randomize while using the Loading Zone randomizer
  if (LZRando)
  {
    NewSystemFlagValue &= ~(1 << 0); // Turn off the 0 bit
    
    // Reset a flag that should always be reset when loading a file
    DefeatedRawkHawk = false;
    
    // Set a flag to reset the System Flag later
    ResetSystemFlag = true;
    
    // Set a flag to manually close the curtain upon loading a file, if Mario comes out of a pipe
    CloseCurtainFlag = true;
    
    // Clear a flag to allow the player to interact with stuff
    uint32_t GSW = reinterpret_cast<uint32_t>(GSWAddress);
    *reinterpret_cast<uint32_t *>(GSW + 0x18) = 0;
  }
  
  // Store the new value
  *reinterpret_cast<uint32_t *>(GSWAddress) = NewSystemFlagValue;
  
  return swByteGetCheck;
}
}

extern "C" {
bool resetSystemFlag(void *GSWAddress)
{
  uint32_t NewSystemFlagValue = *reinterpret_cast<uint32_t *>(GSWAddress);
  
  // Only reset if ResetSystemFlag is on and while using the Loading Zone randomizer
  if (ResetSystemFlag && LZRando)
  {
    ResetSystemFlag = false;
    
    // Only reset if GSW(4) is set to 21, as this is the value used when loading a file that has just saved after the credits
    uint32_t GSW_4 = ttyd::swdrv::swByteGet(4);
    if (GSW_4 == 21)
    {
      NewSystemFlagValue |= (1 << 0); // Turn on the 0 bit
      
      // Store the new value
      *reinterpret_cast<uint32_t *>(GSWAddress) = NewSystemFlagValue;
    }
  }
  
  return (NewSystemFlagValue & (1 << 0)); // Check if the 0 bit is on
}
}

extern "C" {
// The function needs to be different for JP
#ifndef TTYD_JP
uint32_t adjustTitleScreenTimer(uint32_t currentTime)
{
  // Only adjust the timer if the player gets a Game Over and is using the Loading Zone randomizer
  if (GameOverFlag && LZRando)
  {
    return (currentTime * 8); // 7 seconds
  }
  else
  {
    // Return the original value
    return (currentTime * 20); // 19 seconds
  }
}
#else
int32_t adjustTitleScreenTimer(uint32_t currentTime, void *titleWorkPointer2)
{
  // Store the current time
  uint32_t tempTitleWorkPointer2 = reinterpret_cast<uint32_t>(titleWorkPointer2);
  *reinterpret_cast<uint32_t *>(tempTitleWorkPointer2 + 0x28) = currentTime;
  
  // Only adjust the timer if the player gets a Game Over and is using the Loading Zone randomizer
  if (GameOverFlag && LZRando)
  {
    return (currentTime - 420); // 7 seconds
  }
  else
  {
    // Return the original value
    return (currentTime - 1140); // 19 seconds
  }
}
#endif
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
  // Only prevent from running if the challenge mode is currently in use
  if (LZRandoChallenge)
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
  // Prevent getting a Game Over if the player said No to the Shadow Queen
  if (SQWarpAwayCounter > 0)
  {
    // Only run this code once, as it runs multiple times during a transition, and it will interfere with other code if it sets the Sequence more than once
    if (SQWarpAwayCounter == 1)
    {
      // Increment the counter so that the Sequence isn't set multiple times
      SQWarpAwayCounter = 2;
      
      // Set the Sequence to post game
      ttyd::swdrv::swByteSet(0, 405);
      
      // Warp to a different room
      ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "las_29", "w_bero");
    }
    
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
  // Only prevent from running if currently reloading a room and if the challenge mode is currently in use
  if (ReloadCurrentScreen && LZRandoChallenge)
  {
    return false;
  }
  else
  {
    // Call original function
    return mPFN_preventGetItemOnReload_trampoline(id);
  }
}

void Mod::preventSpecificFades(int type, int duration, uint8_t color[4])
{
  // Only prevent from displaying if the Loading Zone randomizer is currently in use
  if (LZRando)
  {
    const int BlackMarioHeadFadeIn = 1;
    const int BlackMarioHeadFadeOut = 2;
    const int MarioHeadFadeIn = 54;
    const int MarioHeadFadeOut = 55;
    const int MarioHeadAfterCreditsFadeOut = 60;
    const int BlackFadeIn = 9;
    const int BlackFadeOut = 10;
    
    switch (type)
    {
      case BlackMarioHeadFadeIn:
      {
        // Clear the black screen
        type = BlackFadeIn;
        
        // Change the duration to the default value used for BlackFadeIn
        duration = 300;
        break;
      }
      case BlackMarioHeadFadeOut:
      {
        // Change the black Mario head to a black screen
        type = BlackFadeOut;
        
        // Change the duration to the default value used for BlackFadeOut
        duration = 300;
        break;
      }
      case MarioHeadFadeIn:
      {
        // Clear the black screen
        type = BlackFadeIn;
        
        // Change the duration to 0 to set the screen to black instantly
        duration = 0;
        break;
      }
      case MarioHeadFadeOut:
      {
        // Change the Mario heads to a black screen
        type = BlackFadeOut;
        
        // Change the duration to 0 so that the black screen goes away instantly
        duration = 0;
        break;
      }
      case MarioHeadAfterCreditsFadeOut:
      {
        // Replace the Mario endgame heads with a black screen
        type = BlackFadeOut;
        
        // Change the duration to 0 so that the black screen goes away instantly
        duration = 0;
        break;
      }
      default:
      {
        break;
      }
    }
  }
  
  // Call original function
  mPFN_preventSpecificFades_trampoline(type, duration, color);
}

void Mod::writeLZRandoAssemblyPatches()
{
  #ifdef TTYD_US
    uint32_t RandomWarp = 0x800087C8;
    uint32_t TubeModeCheck = 0x80092344;
    uint32_t PaperModeCheck = 0x800A6A3C;
    uint32_t BoatModeCheck = 0x8009268C;
    uint32_t PlaneModeCheck = 0x80092748;
    uint32_t MarioKeyOn = 0x8005C1C8;
    uint32_t WalkOnWater = 0x8008F940;
    uint32_t LandOnWater = 0x80092DF4;
    uint32_t SpawnPartnerInBattle = 0x800F8B44;
    uint32_t PreventBattleOnRespawn = 0x800465CC;
    uint32_t CheckCurrentTextbox = 0x800D2880;
    uint32_t PowerRushIconCheck = 0x8001A320;
    uint32_t PowerRushPIconCheck = 0x8001A314;
    uint32_t ReplaceJumpFallAnim = 0x800411D0;
    uint32_t RandomizeBeroFileLoad = 0x800F3A70;
    uint32_t ResetSystemFlag = 0x8000847C;
    uint32_t AdjustTitleScreenTimer = 0x800096B4;
  #elif defined TTYD_JP
    uint32_t RandomWarp = 0x800086F0;
    uint32_t TubeModeCheck = 0x80090D90;
    uint32_t PaperModeCheck = 0x800A4DB4;
    uint32_t BoatModeCheck = 0x800910D8;
    uint32_t PlaneModeCheck = 0x80091194;
    uint32_t MarioKeyOn = 0x8005B370;
    uint32_t WalkOnWater = 0x8008E38C;
    uint32_t LandOnWater = 0x80091840;
    uint32_t SpawnPartnerInBattle = 0x800F3BF4;
    uint32_t PreventBattleOnRespawn = 0x80045F28;
    uint32_t CheckCurrentTextbox = 0x800CE750;
    uint32_t ReplaceJumpFallAnim = 0x80040B34;
    uint32_t RandomizeBeroFileLoad = 0x800EEDB4;
    uint32_t ResetSystemFlag = 0x800083A4;
    uint32_t AdjustTitleScreenTimer = 0x80009538;
  #elif defined TTYD_EU
    uint32_t RandomWarp = 0x80008994;
    uint32_t TubeModeCheck = 0x800936A0;
    uint32_t PaperModeCheck = 0x800A7E0C;
    uint32_t BoatModeCheck = 0x800939E8;
    uint32_t PlaneModeCheck = 0x80093AA4;
    uint32_t MarioKeyOn = 0x8005C300;
    uint32_t WalkOnWater = 0x80090C9C;
    uint32_t LandOnWater = 0x80094170;
    uint32_t JumpOnWater = 0x80093CFC;
    uint32_t SpawnPartnerInBattle = 0x800F99B0;
    uint32_t PreventBattleOnRespawn = 0x800466B4;
    uint32_t CheckCurrentTextbox = 0x800D3678;
    uint32_t PowerRushIconCheck = 0x8001A4E4;
    uint32_t PowerRushPIconCheck = 0x8001A4D8;
    uint32_t ReplaceJumpFallAnim = 0x800412B8;
    uint32_t RandomizeBeroFileLoad = 0x800F48DC;
    uint32_t ResetSystemFlag = 0x80008648;
    uint32_t AdjustTitleScreenTimer = 0x80009878;
  #endif
  
  // Random Warps
  patch::writeBranch(reinterpret_cast<void *>(RandomWarp), reinterpret_cast<void *>(StartGetRandomWarp));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackGetRandomWarp), reinterpret_cast<void *>(RandomWarp + 0x4));
  
  // Allow Paper and Tube modes to be used in multiple rooms
  patch::writeBranch(reinterpret_cast<void *>(TubeModeCheck), reinterpret_cast<void *>(StartEnablePaperTubeModes));
  patch::writeBranch(reinterpret_cast<void *>(PaperModeCheck), reinterpret_cast<void *>(StartEnablePaperTubeModes));
  
  // Adjust branches to be bl instead of b; should modify the patch function to allow for this instead
  *reinterpret_cast<uint32_t *>(TubeModeCheck) += 0x1;
  *reinterpret_cast<uint32_t *>(PaperModeCheck) += 0x1;
  
  // Allow Boat mode to be used in multiple maps
  patch::writeBranch(reinterpret_cast<void *>(BoatModeCheck), reinterpret_cast<void *>(StartEnableBoatMode));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackEnableBoatMode), reinterpret_cast<void *>(BoatModeCheck + 0x4));
  
  // Allow Plane mode to be used in multiple rooms
  patch::writeBranch(reinterpret_cast<void *>(PlaneModeCheck), reinterpret_cast<void *>(StartEnablePlaneMode));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackEnablePlaneMode), reinterpret_cast<void *>(PlaneModeCheck + 0x4));
  
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
  
  // Randomize the loading zone upon loading a file
  patch::writeBranch(reinterpret_cast<void *>(RandomizeBeroFileLoad), reinterpret_cast<void *>(StartRandomizeBeroOnFileLoad));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackRandomizeBeroOnFileLoad), reinterpret_cast<void *>(RandomizeBeroFileLoad + 0x4));
  
  // Reset the System Flag if it was modified when loading a file
  patch::writeBranch(reinterpret_cast<void *>(ResetSystemFlag), reinterpret_cast<void *>(StartResetSystemFlag));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackResetSystemFlag), reinterpret_cast<void *>(ResetSystemFlag + 0x4));
  
  // Adjust the Title Screen timer when the player gets a Game Over and is using the Loading Zone randomizer
  patch::writeBranch(reinterpret_cast<void *>(AdjustTitleScreenTimer), reinterpret_cast<void *>(StartAdjustTitleScreenTimer));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackAdjustTitleScreenTimer), reinterpret_cast<void *>(AdjustTitleScreenTimer + 0x4));
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