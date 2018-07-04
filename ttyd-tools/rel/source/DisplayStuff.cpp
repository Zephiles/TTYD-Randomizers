#include "mod.h"
#include "items.h"
#include "buttons.h"

#include <ttyd/seqdrv.h>
#include <ttyd/swdrv.h>
#include <ttyd/fontmgr.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/party.h>
#include <ttyd/mario_party.h>
#include <ttyd/string.h>
#include <ttyd/__mem.h>
#include <ttyd/system.h>

#include <cstdio>

extern char *NextBero;
extern char *NextMap;
extern uint32_t GSWAddressesStart;
extern bool InCredits;
extern uint16_t CreditsCount;
extern bool BossDefeated[11];
extern uint16_t BossCount;
extern bool TimerActive;
extern uint32_t TimerCount;
extern uint32_t seqMainAddress;
extern bool DenyInput;
extern bool ItemRandoV2;
extern bool LZRando;
extern bool LZRandoChallenge;
extern char *LZRandoText;
extern char *LZRandoChallengeText;

namespace mod {

void Mod::LZRandoStuff()
{
  uint32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  uint32_t Game = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame);
  uint32_t MapChange = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kMapChange);
  
  if ((NextSeq >= Game) && (NextSeq <= MapChange))
  {
    uint32_t color = 0xFFFFFFFF;
    int32_t PosX = -232;
    int32_t PosY = -93;
    
    sprintf(mDisplayBuffer,
        "Seq: %lu\r\nLZ: %s\r\nMap: %s",
        ttyd::swdrv::swByteGet(0),
        NextBero,
        NextMap);
      
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawMessage(PosX, PosY, mDisplayBuffer);
  }
}

void Mod::LZRandoChallengeStuff()
{
  uint32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  uint32_t Game = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame);
  uint32_t MapChange = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kMapChange);
  uint32_t Battle = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kBattle);
  uint32_t Load = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kLoad);
  uint32_t color = 0xFFFFFFFF;
  
  // Get and display Score
  if ((NextSeq >= Game) && (NextSeq <= MapChange))
  {
    uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
    uint32_t PouchPointer = reinterpret_cast<uint32_t>(ttyd::mario_pouch::pouchGetPtr());
    uint32_t Score = 0;
    
    // Check Important Items
    uint32_t ImportantItemsAddressesStart = PouchPointer + 0xA0;
    for (int i = 0; i < 121; i++)
    {
      uint16_t ImportantItem = *reinterpret_cast<uint16_t *>(ImportantItemsAddressesStart + (i * 0x2));
      if ((ImportantItem != Hammer) && (ImportantItem != MagicalMap) && (ImportantItem != Boots) && (ImportantItem != MailboxSP))
      {
        if ((ImportantItem >= DiamondStar) && (ImportantItem <= CrystalStar))
        {
          // Add 2 points for a crystal star
          Score += 2;
        }
        else if ((ImportantItem >= PaperModeCurse) && (ImportantItem <= BoatModeCurse))
        {
          // Add 6 points for curses
          Score += 6;
        }
        else if (ImportantItem == 0)
        {
          // No more important items, so exit loop
          break;
        }
        else
        {
          // Generic Important Item, so add 1 point
          Score++;
        }
      }
    }
    
    // Check for a follower
    uint32_t FollowerPointer = reinterpret_cast<uint32_t>(ttyd::party::anotherPartyGetPtr(ttyd::mario_party::marioGetPartyId()));
    if (FollowerPointer)
    {
      // Add 2 points if the player has a follower
      Score += 2;
    }
    
    // Check for level ups
    uint16_t MarioLevel = *reinterpret_cast<uint16_t *>(PouchPointer + 0x8A);
    if (MarioLevel > 1)
    {
      // Add 3 points for each level up
      Score += (MarioLevel - 1) * 3;
    }
    
    // Check for credits warps
    int32_t CreditsCheck = ttyd::string::strcmp(NextMap, "end_00");
    if (!InCredits && (CreditsCheck == 0))
    {
      InCredits = true;
      CreditsCount++;
    }
    else if (CreditsCheck != 0)
    {
      InCredits = false;
    }
    
    // Add 5 points for each credits warp
    Score += CreditsCount * 5;
    
    // Check for bosses
    uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
    uint32_t PitFlagsAddress = GSWAddresses + 0x3F0;
    
    if (((*reinterpret_cast<uint32_t *>(PitFlagsAddress)) & (1 << 29)) && !BossDefeated[0]) // Check GSWF(5085) - Check the 29 bit
    {
      // Bonetail has been defeated
      BossDefeated[0] = true;
      BossCount++;
    }
    else if ((SequencePosition == 21) && !BossDefeated[1])
    {
      // Blooper has been defeated
      BossDefeated[1] = true;
      BossCount++;
    }
    else if ((SequencePosition == 56) && !BossDefeated[2])
    {
      // Hooktail has been defeated
      BossDefeated[2] = true;
      BossCount++;
    }
    else if ((SequencePosition == 112) && !BossDefeated[3])
    {
      // Magnus 1.0 has been defeated
      BossDefeated[3] = true;
      BossCount++;
    }
    else if ((SequencePosition == 164) && !BossDefeated[4])
    {
      // Grubba has been defeated
      BossDefeated[4] = true;
      BossCount++;
    }
    else if ((SequencePosition == 200) && !BossDefeated[5])
    {
      // Doopliss 1 has been defeated
      BossDefeated[5] = true;
      BossCount++;
    }
    else if ((SequencePosition == 211) && !BossDefeated[6])
    {
      // Doopliss 2 has been defeated
      BossDefeated[6] = true;
      BossCount++;
    }
    else if ((SequencePosition == 253) && !BossDefeated[7])
    {
      // Cortez has been defeated
      BossDefeated[7] = true;
      BossCount++;
    }
    else if ((SequencePosition == 332) && !BossDefeated[8])
    {
      // Smorg has been defeated
      BossDefeated[8] = true;
      BossCount++;
    }
    else if ((SequencePosition == 373) && !BossDefeated[9])
    {
      // Magnus 2.0 has been defeated
      BossDefeated[9] = true;
      BossCount++;
    }
    else if ((SequencePosition == 401) && !BossDefeated[10])
    {
      // Shadow Queen has been defeated
      BossDefeated[10] = true;
      BossCount++;
    }
    
    // Add 10 points for each boss defeated
    Score += BossCount * 10;
    
    // Add 1 point for Mario's coin count divided by 100
    uint16_t CoinCount = *reinterpret_cast<uint16_t *>(PouchPointer + 0x78);
    Score += CoinCount / 100;
    
    // Check badge log
    uint32_t BadgeLogAddressesStart = GSWAddresses + 0x188;
    uint32_t BadgeLogCount = 0;
    
    // Check each bit in the bitfield
    for (int i = 0; i < 3; i++)
    {
      for (int x = 0; x < 32; x++)
      {
        uint32_t CurrentAddress = *reinterpret_cast<uint32_t *>(BadgeLogAddressesStart + (i * 0x4));
        if (CurrentAddress & (1 << x))
        {
          // Add 1 to the count if the bit is on
          BadgeLogCount++;
        }
      }
    }
    
    // Add 1 point for the badge log count divided by 10
    Score += BadgeLogCount / 10;
    
    // Display Score
    int32_t PosX = -232;
    int32_t PosY = -65;
    
    sprintf(mDisplayBuffer,
      "Score: %ld",
      Score);
      
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawMessage(PosX, PosY, mDisplayBuffer);
  }
  
  // Display timer
  if (TimerActive || ((NextSeq >= Game) && (NextSeq <= Battle)))
  {
    int32_t PosX = 85;
    int32_t PosY = -153;
    
    #ifdef TTYD_JP
      PosX += 15;
    #endif
    
    uint32_t modframe = TimerCount % 60;
    uint32_t second = (TimerCount / 60) % 60;
    uint32_t minute = (TimerCount / (60 * 60)) % 60;
    uint32_t hour = TimerCount / (60 * 60 * 60);
    
    sprintf(mDisplayBuffer,
      "%.1ld:%.2ld:%.2ld.%.2ld",
      hour,
      minute,
      second,
      modframe);
    
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawMessage(PosX, PosY, mDisplayBuffer);
    
    // Activate timer
    if (!TimerActive)
    {
      TimerCount = 216000; // 1 hour
      
      if (NextSeq == Game)
      {
        TimerActive = true;
      }
    }
    else
    {
      if (TimerCount > 0)
      {
        // Run timer
        TimerCount--;
      }
      else // TimerCount == 0
      {
        // Display text for time being up
        PosX = -90;
        PosY = 20;
        float Scale = 1.5;
        
        sprintf(mDisplayBuffer,
          "%s",
          "Time's Up!");
        
        ttyd::fontmgr::FontDrawStart();
        ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
        ttyd::fontmgr::FontDrawEdge();
        ttyd::fontmgr::FontDrawScale(Scale);
        ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
      }
    }
  }
  
  // Disable timer
  if (TimerActive && (NextSeq == Load))
  {
    TimerActive = false;
  }
  
  // Reset values upon going to the file select screen
  if (NextSeq == Load)
  {
    CreditsCount = 0;
    BossCount = 0;
    ttyd::__mem::memset(BossDefeated, false, sizeof(BossDefeated));
  }
}

void Mod::changeGameModes()
{
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  
  // Only run on the file select screen whule the curtain is up
  if ((ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kLoad)) && (seqMainCheck == 2))
  {
    uint32_t ButtonInput = ttyd::system::keyGetButton(0);
    uint16_t ItemRandoButtonCombo = PAD_L | PAD_Y;
    uint16_t LZRandoButtonCombo = PAD_L | PAD_X;
    uint16_t LZRandoChallengeButtonCombo = PAD_L | PAD_Z;
    
    if ((ButtonInput & ItemRandoButtonCombo) == ItemRandoButtonCombo)
    {
      if (!DenyInput)
      {
        ItemRandoV2 = !ItemRandoV2;
      }
      DenyInput = true;
    }
    else if ((ButtonInput & LZRandoButtonCombo) == LZRandoButtonCombo)
    {
      if (!DenyInput)
      {
        LZRando = !LZRando;
      }
      DenyInput = true;
    }
    else if ((ButtonInput & LZRandoChallengeButtonCombo) == LZRandoChallengeButtonCombo)
    {
      if (!DenyInput)
      {
        LZRandoChallenge = !LZRandoChallenge;
      }
      DenyInput = true;
    }
    else
    {
      // Reset flag if no button combo is pressed/held
      DenyInput = false;
    }
  }
}

void Mod::titleScreenStuff()
{
  // Only run on the title screen
  if (ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kTitle))
  {
    uint32_t color = 0xFFFFFFFF;
    int32_t PosX = -235;
    int32_t PosY = -25;
    float Scale = 0.9;
    
    #ifdef TTYD_JP
      PosX += 10;
      PosY += 30;
    #endif
    
    sprintf(mDisplayBuffer,
      "%s",
      "Item Randomizer V1 - v1.2");
    
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
    
    sprintf(mDisplayBuffer,
      "%s",
      "Item Randomizer V2 - v1.2");
    
    PosY -= 20;
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
    
    sprintf(mDisplayBuffer,
      "%s",
      "Loading Zone Randomizer Beta - v0.5");
    
    PosY -= 20;
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
  }
}

void Mod::gameModes()
{
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  
  // Only run on the file select screen whule the curtain is up
  if ((ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kLoad)) && (seqMainCheck == 2))
  {
    uint32_t color = 0xFFFFFFFF;
    int32_t PosX = -255;
    int32_t PosY = 0;
    float Scale = 0.75;
    
    // Toggle Mode text
    sprintf(mDisplayBuffer,
      "%s",
      "Hold L and press the following to toggle modes:");
    
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
    
    sprintf(mDisplayBuffer,
      "%s",
      "Y = Item Randomizer v1/v2");
    
    PosY -= 20;
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
    
    sprintf(mDisplayBuffer,
      "%s",
      "X = Loading Zone Randomizer Beta");
    
    PosY -= 20;
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
    
    sprintf(mDisplayBuffer,
      "%s",
      "Z = Loading Zone Randomizer - 1 Hour Challenge");
    
    PosY -= 20;
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
    
    // Display Item Randomizer mode
    sprintf(mDisplayBuffer,
      "%s%ld",
      "Item Randomizer: v",
      (static_cast<uint32_t>(ItemRandoV2)) + 1);
    
    PosY -= 50;
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
    
    // Display Loading Zone Randomizer Beta on or off
    if (LZRando)
    {
      ttyd::string::strcpy(LZRandoText, "On");
    }
    else
    {
      ttyd::string::strcpy(LZRandoText, "Off");
    }
    
    sprintf(mDisplayBuffer,
      "%s%s",
      "Loading Zone Randomizer: ",
      LZRandoText);
    
    PosY -= 20;
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
    
    // Display Loading Zone Randomizer 1 Hour Challenge on or off
    // If the Loading Zone Randomizer is off, then turn the 1 Hour Challenge off
    if (!LZRando)
    {
      LZRandoChallenge = false;
    }
    
    if (LZRandoChallenge)
    {
      ttyd::string::strcpy(LZRandoChallengeText, "On");
    }
    else
    {
      ttyd::string::strcpy(LZRandoChallengeText, "Off");
    }
    
    sprintf(mDisplayBuffer,
      "%s%s",
      "Loading Zone Randomizer - 1 Hour Challenge: ",
      LZRandoChallengeText);
    
    PosY -= 20;
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
  }
}

}