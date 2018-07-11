#include "mod.h"
#include "drawstring.h"
#include "items.h"
#include "buttons.h"

#include <ttyd/seqdrv.h>
#include <ttyd/string.h>
#include <ttyd/swdrv.h>
#include <ttyd/fontmgr.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/party.h>
#include <ttyd/mario_party.h>
#include <ttyd/system.h>
#include <ttyd/__mem.h>

#include <cstdio>

extern char *NextMap;
extern char *NextBero;
extern uint32_t GSWAddressesStart;
extern bool InCredits;
extern uint16_t CreditsCount;
extern bool BossDefeated[11];
extern uint16_t BossCount;
extern bool TimerDisabled;
extern bool TimerActive;
extern uint32_t TimerCount;
extern bool DisplayTimer;
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
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  int32_t MapChange = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kMapChange);
  
  bool dmo_comparison = ttyd::string::strcmp(NextMap,"dmo_00") == 0;
  bool title_comparison = ttyd::string::strcmp(NextMap,"title") == 0;
  
  // Only display while a file is loaded, and while not in battles
  // Don't display while in the intro
  if ((NextSeq < Game) || (NextSeq > MapChange) || dmo_comparison || title_comparison)
  {
    return;
  }
  
  uint32_t color = 0xFFFFFFFF;
  int32_t PosX = -232;
  int32_t PosY = -120;
  float Scale = 0.75;
  
  sprintf(mDisplayBuffer,
    "Seq: %lu\nLZ: %s\nMap: %s",
    ttyd::swdrv::swByteGet(0),
    NextBero,
    NextMap);
    
  ttyd::fontmgr::FontDrawStart();
  ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
  ttyd::fontmgr::FontDrawEdge();
  ttyd::fontmgr::FontDrawScale(Scale);
  drawstring::drawStringMultiline(PosX, PosY, mDisplayBuffer);
}

void Mod::LZRandoChallengeStuff()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Seq = ttyd::seqdrv::seqGetSeq();
  int32_t Title = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kTitle);
  int32_t Game = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGame);
  int32_t MapChange = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kMapChange);
  int32_t Battle = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kBattle);
  int32_t GameOver = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kGameOver);
  int32_t Load = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kLoad);
  uint32_t color = 0xFFFFFFFF;
  
  bool dmo_comparison = ttyd::string::strcmp(NextMap,"dmo_00") != 0;
  bool title_comparison = ttyd::string::strcmp(NextMap,"title") != 0;
  
  // Get and display Score
  // Don't display while in the intro or on the title screen
  if ((NextSeq >= Game) && (NextSeq <= MapChange) && dmo_comparison && title_comparison)
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
        if ((ImportantItem == MagicalMapBigger) || ((ImportantItem >= DiamondStar) && (ImportantItem <= CrystalStar)))
        {
          // Add 2 points for the magical map or a crystal star
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
    uint32_t FollowerPointer = reinterpret_cast<uint32_t>(ttyd::party::partyGetPtr(ttyd::mario_party::marioGetExtraPartyId()));
    if (FollowerPointer)
    {
      // Add 2 points if the player has a follower
      Score += 2;
    }
    
    // Check for level ups
    int16_t MarioLevel = *reinterpret_cast<int16_t *>(PouchPointer + 0x8A);
    if (MarioLevel > 1)
    {
      // Add 3 points for each level up
      Score += (MarioLevel - 1) * 3;
    }
    
    // Check for credits warps
    bool CreditsCheck = ttyd::string::strcmp(NextMap, "end_00") == 0;
    if (!InCredits && CreditsCheck)
    {
      InCredits = true;
      CreditsCount++;
    }
    else if (!CreditsCheck)
    {
      InCredits = false;
    }
    
    // Add 5 points for each credits warp
    Score += CreditsCount * 5;
    
    // Check for bosses
    uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
    bool BonetailCheck = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x3F0) & (1 << 29); // Check GSWF(5085) - Check the 29 bit
    
    if (BonetailCheck && !BossDefeated[0])
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
    int16_t CoinCount = *reinterpret_cast<int16_t *>(PouchPointer + 0x78);
    Score += CoinCount / 100;
    
    // Check badge log
    uint32_t BadgeLogAddressesStart = GSWAddresses + 0x188;
    uint32_t BadgeLogCount = 0;
    
    // Check each bit in the bitfield
    for (int i = 0; i < 3; i++)
    {
      uint32_t CurrentAddress = *reinterpret_cast<uint32_t *>(BadgeLogAddressesStart + (i * 0x4));
      for (int x = 0; x < 32; x++)
      {
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
    int32_t PosY = -100;
    float Scale = 0.75;
    
    sprintf(mDisplayBuffer,
      "Score: %ld",
      Score);
      
    ttyd::fontmgr::FontDrawStart();
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawEdge();
    ttyd::fontmgr::FontDrawScale(Scale);
    ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
  }
  
  // Don't display timer if disabled
  if (!TimerDisabled)
  {
    // Activate timer
    if (!TimerActive)
    {
      TimerCount = 216000; // 1 hour
      
      // Don't activate timer if in the intro or on the title screen
      if (dmo_comparison && title_comparison)
      {
        if (NextSeq == MapChange)
        {
          // Display timer
          DisplayTimer = true;
        }
        else if (NextSeq == Game)
        {
          // Activate timer
          TimerActive = true;
        }
      }
    }
    
    // Display timer
    if (DisplayTimer && (NextSeq >= Title) && (NextSeq <= GameOver))
    {
      bool Comparisons = (NextSeq == Title) && (TimerCount == 0);
      
      // Don't show the timer on the title screen while it is at 0
      if (!Comparisons)
      {
        int32_t FontDrawX = 105;
        int32_t FontDrawY = -160;
        float Scale = 0.8;
        
        #ifdef TTYD_JP
          FontDrawX += 5;
          FontDrawY += 2;
          Scale += 0.05;
        #endif
        
        uint32_t modframe = TimerCount % 60;
        uint32_t second = (TimerCount / 60) % 60;
        uint32_t minute = (TimerCount / (60 * 60)) % 60;
        uint32_t hour = TimerCount / (60 * 60 * 60);
        
        sprintf(mDisplayBuffer,
          "%.2ld:%.2ld:%.2ld.%.2ld",
          hour,
          minute,
          second,
          modframe);
        
        ttyd::fontmgr::FontDrawStart();
        uint32_t color = 0xFFFFFFFF;
        ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
        ttyd::fontmgr::FontDrawEdge();
        ttyd::fontmgr::FontDrawScale(Scale);
        ttyd::fontmgr::FontDrawString(FontDrawX, FontDrawY, mDisplayBuffer);
      }
      
      if (TimerActive)
      {
        if (TimerCount > 0)
        {
          // Run timer
          TimerCount--;
        }
        else if ((Seq >= Game) && (NextSeq != Battle))
        {
          // Don't display while on the title screen
          // Don't display in battles, as tyring to warp as the curtain is coming up will crash the game
          // Display text for time being up
          int32_t PosX = -90;
          int32_t PosY = 80;
          float Scale = 1.5;
          
          sprintf(mDisplayBuffer,
            "%s",
            "Time's Up!");
          
          ttyd::fontmgr::FontDrawStart();
          ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
          ttyd::fontmgr::FontDrawEdge();
          ttyd::fontmgr::FontDrawScale(Scale);
          ttyd::fontmgr::FontDrawString(PosX, PosY, mDisplayBuffer);
          
          // Display text for how the player wants to continue
          PosX = -225;
          PosY = 25;
          Scale = 0.9;
          
          sprintf(mDisplayBuffer,
            "%s\n%s",
            "Press Y to continue playing",
            "Press X to return to the title screen");
          
          ttyd::fontmgr::FontDrawStart();
          ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
          ttyd::fontmgr::FontDrawEdge();
          ttyd::fontmgr::FontDrawScale(Scale);
          drawstring::drawStringMultiline(PosX, PosY, mDisplayBuffer);
          
          // Get input for what to do next
          uint32_t ButtonInput = ttyd::system::keyGetButton(0);
          
          if ((ButtonInput & PAD_Y) == PAD_Y)
          {
            // Continue playing without the timer
            TimerDisabled = true;
          }
          else if ((ButtonInput & PAD_X) == PAD_X)
          {
            // Return to the title screen
            TimerDisabled = true;
            ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "title", nullptr);
          }
        }
      }
    }
  }
  
  // Reset values upon going to the file select screen
  if (NextSeq == Load)
  {
    DisplayTimer = false;
    TimerActive = false;
    TimerDisabled = false;
    CreditsCount = 0;
    BossCount = 0;
    ttyd::__mem::memset(BossDefeated, false, sizeof(BossDefeated));
  }
}

void Mod::titleScreenStuff()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Title = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kTitle);
  
  // Only run on the title screen
  if (NextSeq != Title)
  {
    return;
  }
  
  uint32_t color = 0xFFFFFFFF;
  int32_t PosX = -240;
  int32_t PosY = -35;
  float Scale = 0.9;
  
  #ifdef TTYD_JP
    PosX += 10;
    PosY += 30;
  #endif
  
  sprintf(mDisplayBuffer,
    "%s\n%s",
    "Item Randomizers - v1.2.1",
    "Loading Zone Randomizer Beta - v0.5.1");
  
  ttyd::fontmgr::FontDrawStart();
  ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
  ttyd::fontmgr::FontDrawEdge();
  ttyd::fontmgr::FontDrawScale(Scale);
  drawstring::drawStringMultiline(PosX, PosY, mDisplayBuffer);
}

void Mod::gameModes()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Load = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kLoad);
  uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);
  
  bool Comparisons = (NextSeq == Load) && (seqMainCheck == 2);
  
  // Only run on the file select screen while the curtain is up
  if (!Comparisons)
  {
    return;
  }
  
  uint32_t color = 0xFFFFFFFF;
  int32_t PosX = -255;
  int32_t PosY = 40;
  float Scale = 0.75;
  
  // Display game modes
  sprintf(mDisplayBuffer,
    "%s\n%s\n%s\n%s",
    "Hold L and press the following to toggle modes:",
    "Y = Item Randomizer v1/v2",
    "X = Loading Zone Randomizer Beta",
    "Z = Loading Zone Randomizer - 1 Hour Challenge");
  
  ttyd::fontmgr::FontDrawStart();
  ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
  ttyd::fontmgr::FontDrawEdge();
  ttyd::fontmgr::FontDrawScale(Scale);
  drawstring::drawStringMultiline(PosX, PosY, mDisplayBuffer);
  
  // Set LZRandoText to the appropriate value
  if (LZRando)
  {
    ttyd::string::strcpy(LZRandoText, "On");
  }
  else
  {
    ttyd::string::strcpy(LZRandoText, "Off");
    
    // If the Loading Zone Randomizer is off, then turn the 1 Hour Challenge off
    LZRandoChallenge = false;
  }
  
  // Set LZRandoChallengeText to the appropriate value
  if (LZRandoChallenge)
  {
    ttyd::string::strcpy(LZRandoChallengeText, "On");
  }
  else
  {
    ttyd::string::strcpy(LZRandoChallengeText, "Off");
  }
  
  // Move the text down
  PosY -= 120;
  
  // Display current modes
  sprintf(mDisplayBuffer,
    "%s%ld\n%s%s\n%s%s",
    "Item Randomizer: v",
    static_cast<uint32_t>(ItemRandoV2) + 1,
    "Loading Zone Randomizer: ",
    LZRandoText,
    "Loading Zone Randomizer - 1 Hour Challenge: ",
    LZRandoChallengeText);
    
  ttyd::fontmgr::FontDrawStart();
  ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
  ttyd::fontmgr::FontDrawEdge();
  ttyd::fontmgr::FontDrawScale(Scale);
  drawstring::drawStringMultiline(PosX, PosY, mDisplayBuffer);
  
  // Get input for game modes
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