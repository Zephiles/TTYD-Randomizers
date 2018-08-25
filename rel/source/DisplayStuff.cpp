#include "mod.h"
#include "drawstring.h"
#include "items.h"
#include "buttons.h"

#include <ttyd/fontmgr.h>
#include <ttyd/seqdrv.h>
#include <ttyd/string.h>
#include <ttyd/swdrv.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/mario_party.h>
#include <ttyd/system.h>
#include <ttyd/__mem.h>
#include <ttyd/mariost.h>
#include <ttyd/mario.h>
#include <ttyd/windowdrv.h>
#include <ttyd/dispdrv.h>

#include <cstdio>

extern char *DisplayBuffer;
extern char *NextMap;
extern char *NextBero;
extern uint8_t TimesUpCounter;
extern uint32_t GSWAddressesStart;
extern uint8_t JustDefeatedBoss;
extern bool BossDefeated[17];
extern uint16_t BossCount;
extern bool InGameOver;
extern uint16_t GameOverCount;
extern int32_t FinalScore;
extern int32_t DisplayScores[10];
extern bool ShowScoreSources;
extern bool DenyInput;
extern bool NewFile;
extern uint32_t TimerCount;
extern bool TimerDisabled;
extern bool TimerActive;
extern bool DisablePlayerControl;
extern bool GameOverFlag;
extern uint32_t seqMainAddress;
extern bool LZRando;
extern char *LZRandoText;
extern bool LZRandoChallenge;
extern char *LZRandoChallengeText;
extern bool ItemRandoV2;

namespace mod {

void drawStringSingleLine(int32_t PosX, int32_t PosY, uint32_t color, float Scale)
{
  ttyd::fontmgr::FontDrawStart();
  ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
  ttyd::fontmgr::FontDrawEdge();
  ttyd::fontmgr::FontDrawScale(Scale);
  ttyd::fontmgr::FontDrawString(PosX, PosY, DisplayBuffer);
}

void drawStringMultipleLines(int32_t PosX, int32_t PosY, uint32_t color, float Scale)
{
  ttyd::fontmgr::FontDrawStart();
  ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
  ttyd::fontmgr::FontDrawEdge();
  ttyd::fontmgr::FontDrawScale(Scale);
  drawstring::drawStringMultiline(PosX, PosY, DisplayBuffer);
}

void Mod::LZRandoDisplayStuff()
{
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  ttyd::seqdrv::SeqIndex GameOver = ttyd::seqdrv::SeqIndex::kGameOver;
  
  bool dmo_comparison = ttyd::string::strcmp(NextMap, "dmo_00") == 0;
  bool title_comparison = ttyd::string::strcmp(NextMap, "title") == 0;
  
  // Only display while a file is loaded, and while not in battles
  // Don't display while in the intro or on the title screen
  if ((NextSeq < Game) || (NextSeq > GameOver) || dmo_comparison || title_comparison)
  {
    return;
  }
  
  uint32_t color = 0xFFFFFFFF;
  int32_t PosX = -232;
  int32_t PosY = -120;
  float Scale = 0.75;
  
  sprintf(DisplayBuffer,
    "Seq: %lu\nLZ: %s\nMap: %s",
    ttyd::swdrv::swByteGet(0),
    NextBero,
    NextMap);
    
  drawStringMultipleLines(PosX, PosY, color, Scale);
}

void Mod::LZRandoChallengeStuff()
{
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Title = ttyd::seqdrv::SeqIndex::kTitle;
  ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
  ttyd::seqdrv::SeqIndex Battle = ttyd::seqdrv::SeqIndex::kBattle;
  ttyd::seqdrv::SeqIndex GameOver = ttyd::seqdrv::SeqIndex::kGameOver;
  ttyd::seqdrv::SeqIndex Load = ttyd::seqdrv::SeqIndex::kLoad;
  uint32_t color = 0xFFFFFFFF;
  
  bool dmo_comparison = ttyd::string::strcmp(NextMap, "dmo_00") == 0;
  bool title_comparison = ttyd::string::strcmp(NextMap, "title") == 0;
  
  // Get and display Score
  // Don't display while in the intro or on the title screen
  if ((NextSeq >= Game) && (NextSeq <= GameOver) && !dmo_comparison && !title_comparison)
  {
    int32_t Score = 0;
    int32_t MainScores[10] = { 0 };
    
    // Prevent score from updating if time is up
    if (TimesUpCounter < 2)
    {
      uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
      uint32_t PouchPointer = reinterpret_cast<uint32_t>(ttyd::mario_pouch::pouchGetPtr());
      
      // Check Important Items
      uint32_t ImportantItemsAddressesStart = PouchPointer + 0xA0;
      for (int i = 0; i < 121; i++)
      {
        uint16_t ImportantItem = *reinterpret_cast<uint16_t *>(ImportantItemsAddressesStart + (i * 0x2));
        if ((ImportantItem != Hammer) && (ImportantItem != MagicalMap) && (ImportantItem != Boots) && (ImportantItem != MailboxSP) && (ImportantItem != StrangeSack) && (ImportantItem != BattleTrunks))
        {
          if ((ImportantItem == MagicalMapBigger) || ((ImportantItem >= DiamondStar) && (ImportantItem <= CrystalStar)))
          {
            // Add 2 points for the magical map or a crystal star
            MainScores[0] += 2;
          }
          else if ((ImportantItem >= PaperModeCurse) && (ImportantItem <= BoatModeCurse))
          {
            // Add 6 points for curses
            MainScores[1] += 6;
          }
          else if (ImportantItem == 0)
          {
            // No more important items, so exit loop
            break;
          }
          else
          {
            // Generic Important Item, so add 1 point
            MainScores[2]++;
          }
        }
      }
      
      // Add 1 point per 10 Battle Trunks in the player's inventory
      MainScores[2] += ttyd::mario_pouch::pouchCheckItem(BattleTrunks) / 10;
      
      // Check for a follower
      uint32_t FollowerPointer = reinterpret_cast<uint32_t>(ttyd::party::partyGetPtr(ttyd::mario_party::marioGetExtraPartyId()));
      if (FollowerPointer)
      {
        // Add 2 points if the player has a follower
        MainScores[3] = 2;
      }
      
      // Check for level ups
      int16_t MarioLevel = *reinterpret_cast<int16_t *>(PouchPointer + 0x8A);
      if (MarioLevel > 1)
      {
        // Add 3 points for each level up
        MainScores[4] = (MarioLevel - 1) * 2;
      }
      
      // Check for bosses
      uint32_t BossDefeatedIndex = 0;
      
      // Check for bosses based on current textbox
      for (int i = 1; i <= 10; i++)
      {
        if (JustDefeatedBoss == i)
        {
          if (!BossDefeated[BossDefeatedIndex])
          {
            BossDefeated[BossDefeatedIndex] = true;
            BossCount++;
          }
        }
        else
        {
          BossDefeated[BossDefeatedIndex] = false;
        }
        
        BossDefeatedIndex++;
      }
      
      // Check for bosses based on Sequence
      uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
      uint16_t SequenceChecks[] = { 85, 200, 211, 388 };
      int32_t ArraySize = sizeof(SequenceChecks) / sizeof(SequenceChecks[0]);
      
      BossDefeatedIndex++;
      for (int i = 0; i < ArraySize; i++)
      {
        if (SequencePosition == SequenceChecks[i])
        {
          if (!BossDefeated[BossDefeatedIndex])
          {
            BossDefeated[BossDefeatedIndex] = true;
            BossCount++;
          }
        }
        else
        {
          BossDefeated[BossDefeatedIndex] = false;
        }
        
        BossDefeatedIndex++;
      }
      
      // Check for the Shadow Queen
      BossDefeatedIndex++;
      if (SequencePosition == 401)
      {
        if (!BossDefeated[BossDefeatedIndex])
        {
          BossDefeated[BossDefeatedIndex] = true;
          BossCount += 2;
        }
      }
      else
      {
        BossDefeated[BossDefeatedIndex] = false;
      }
      
      // Check for the Atomic Boo
      BossDefeatedIndex++;
      if (ttyd::string::strcmp(NextMap, "jin_00") == 0)
      {
        // Check GSWF(2226) - Check the 18 bit
        bool AtomicBooCheck = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x28C) & (1 << 18);
        
        if (AtomicBooCheck && !BossDefeated[BossDefeatedIndex])
        {
          BossDefeated[BossDefeatedIndex] = true;
          BossCount++;
        }
      }
      else
      {
        BossDefeated[BossDefeatedIndex] = false;
        
        // Turn off GSWF(2226) so that the player can refight the Atomic Boo
        *reinterpret_cast<uint32_t *>(GSWAddresses + 0x28C) &= ~(1 << 18); // Turn off the 18 bit
      }
      
      // Check for Bonetail
      BossDefeatedIndex++;
      if (ttyd::string::strcmp(NextMap, "jon_06") == 0)
      {
        // Check GSWF(5085) - Check the 29 bit
        bool BonetailCheck = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x3F0) & (1 << 29);
        
        if (BonetailCheck && !BossDefeated[BossDefeatedIndex])
        {
          BossDefeated[BossDefeatedIndex] = true;
          BossCount += 2;
        }
      }
      else
      {
        BossDefeated[BossDefeatedIndex] = false;
        
        // Turn off GSWF(5084) and GSWF(5085) so that the player can refight Bonetail
        *reinterpret_cast<uint32_t *>(GSWAddresses + 0x3F0) &= ~((1 << 28) | (1 << 29)); // Turn off the 28 and 29 bits
      }
      
      // Add 10 points for each boss defeated; 20 points for the Shadow Queen and Bonetail
      MainScores[5] = BossCount * 10;
      
      // Add 1 point for Mario's coin count divided by 100
      int16_t CoinCount = *reinterpret_cast<int16_t *>(PouchPointer + 0x78);
      MainScores[6] = CoinCount / 100;
      
      // Add additional point for 999 coins
      if (CoinCount >= 999)
      {
        MainScores[6]++;
      }
      
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
      MainScores[7] = BadgeLogCount / 10;
      
      // Add 5 points for each partner upgrade
      for (int i = 1; i <= 7; i++)
      {
        MainScores[8] += ttyd::mario_pouch::pouchGetPartyAttackLv(i) * 5;
      }
      
      // Check for Game Over
      if (!InGameOver && (NextSeq == GameOver))
      {
        InGameOver = true;
        GameOverCount++;
      }
      else if (NextSeq != GameOver)
      {
        InGameOver = false;
      }
      
      // Subtract 5 points for each Game Over
      MainScores[9] -= GameOverCount * 5;
      
      // Get total score
      for (int i = 0; i < 10; i++)
      {
        Score += MainScores[i];
      }
    }
    
    if (TimesUpCounter == 2)
    {
      // Display final score
      Score = FinalScore;
      
      // Update where each point came from for the final score
      for (int i = 0; i < 10; i++)
      {
        MainScores[i] = DisplayScores[i];
      }
    }
    else if (TimesUpCounter == 1)
    {
      // Update final score
      TimesUpCounter = 2;
      FinalScore = Score;
      
      // Update where each point came from for the final score
      for (int i = 0; i < 10; i++)
      {
        DisplayScores[i] = MainScores[i];
      }
    }
    
    // Display Score
    int32_t PosX = -232;
    int32_t PosY = -100;
    float Scale = 0.75;
    
    sprintf(DisplayBuffer,
      "Score: %ld",
      Score);
      
    drawStringSingleLine(PosX, PosY, color, Scale);
    
    // Display where the points came from
    if (ShowScoreSources)
    {
      PosX = -232;
      PosY = -80;
      
      sprintf(DisplayBuffer,
        "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
        MainScores[0],
        MainScores[1],
        MainScores[2],
        MainScores[3],
        MainScores[4],
        MainScores[5],
        MainScores[6],
        MainScores[7],
        MainScores[8],
        MainScores[9]);
      
      drawStringSingleLine(PosX, PosY, color, Scale);
    }
    
    // Get input for whether to display the score sources or not
    uint32_t ButtonInput = ttyd::system::keyGetButton(0);
    uint16_t ShowScoreSourceCombo = PAD_L | PAD_R;
    
    if ((ButtonInput & ShowScoreSourceCombo) == ShowScoreSourceCombo)
    {
      if (!DenyInput)
      {
        ShowScoreSources = !ShowScoreSources;
      }
      DenyInput = true;
    }
    else
    {
      // Reset flag if no button combo is pressed/held
      DenyInput = false;
    }
  }
  
  // Reset values if starting a new file
  if (NewFile)
  {
    TimerCount = 216000; // 1 hour
    ShowScoreSources = false;
    TimerDisabled = false;
    TimerActive = false;
    DisablePlayerControl = false;
    TimesUpCounter = 0;
    BossCount = 0;
    GameOverCount = 0;
    ttyd::__mem::memset(BossDefeated, false, sizeof(BossDefeated));
  }
  
  // Don't display timer if disabled
  if (!TimerDisabled)
  {
    // Activate timer
    if (!TimerActive)
    {
      // Don't activate timer if in the intro or on the title screen
      if (!dmo_comparison && !title_comparison)
      {
        if (NextSeq == Game)
        {
          // Activate timer
          TimerActive = true;
        }
      }
    }
    
    // Display timer
    ttyd::seqdrv::SeqIndex Seq = ttyd::seqdrv::seqGetSeq();
    bool Comparisons = ((Seq == Title) || (Seq == Load) || dmo_comparison || title_comparison) && (TimerCount == 0);
    
    // Don't show the timer on the title screen or file select screen while it is at 0
    // Don't show the timer during the intro
    if (!Comparisons)
    {
      // Display timer
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
      
      sprintf(DisplayBuffer,
        "%.2ld:%.2ld:%.2ld.%.2ld",
        hour,
        minute,
        second,
        modframe);
      
      drawStringSingleLine(FontDrawX, FontDrawY, color, Scale);
    }
    
    if (TimerActive)
    {
      if (TimerCount > 0)
      {
        // Run timer
        TimerCount--;
      }
      else if ((Seq >= Game) && Seq <= GameOver)
      {
        // Prevent score from updating
        if (TimesUpCounter == 0)
        {
          TimesUpCounter = 1;
        }
        
        uint32_t SystemLevel = ttyd::mariost::marioStGetSystemLevel();
        
        // Pause everything and disable player control
        if (!DisablePlayerControl && (NextSeq == Game) && (SystemLevel != 15))
        {
          DisablePlayerControl = true;
          ttyd::mariost::marioStSystemLevel(1);
          
          // Only turn the key off if it's not already off
          if (ttyd::mario::marioKeyOffChk() == 0)
          {
            ttyd::mario::marioKeyOff();
          }
        }
        
        // Don't display while on the title screen or file select screen
        // Display text for time being up
        int32_t PosX = -90;
        int32_t PosY = 80;
        float Scale = 1.5;
        
        sprintf(DisplayBuffer,
          "Time's Up!");
        
        drawStringSingleLine(PosX, PosY, color, Scale);
        
        // // Don't display or allow modifying of the options in battles, as trying to warp as the curtain is coming up will crash the game
        if (NextSeq != Battle)
        {
          // Display text for how the player wants to continue
          PosX = -225;
          PosY = 25;
          Scale = 0.9;
          
          sprintf(DisplayBuffer,
            "%s\n%s",
            "Press Y to continue playing",
            "Press X to return to the title screen");
          
          drawStringMultipleLines(PosX, PosY, color, Scale);
          
          // Get input for what to do next
          uint32_t ButtonInputTrg = ttyd::system::keyGetButtonTrg(0);
          
          if ((ButtonInputTrg & PAD_Y) == PAD_Y)
          {
            // Continue playing without the timer
            TimerDisabled = true;
            DisablePlayerControl = false;
            ttyd::mariost::marioStSystemLevel(0);
            
            // Only turn the key on if it's not already on
            if (ttyd::mario::marioKeyOffChk() != 0)
            {
              ttyd::mario::marioKeyOn();
            }
          }
          else if ((ButtonInputTrg & PAD_X) == PAD_X)
          {
            // Return to the title screen
            TimerDisabled = true;
            GameOverFlag = false;
            ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "title", nullptr);
            ttyd::mariost::marioStSystemLevel(0);
          }
        }
      }
    }
  }
}

void Mod::titleScreenStuff()
{
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Title = ttyd::seqdrv::SeqIndex::kTitle;
  
  // Only run on the title screen
  if (NextSeq != Title)
  {
    return;
  }
  
  // Display Item Randomizer versions and Loading Zone Randomizer version
  // Draw window for the text to go in
  uint32_t color = 0x000000CC;
  float CoordX = -260;
  float CoordY = -25;
  float Width = 532;
  float Height = 65;
  float Curve = 10;
  
  #ifdef TTYD_JP
    CoordX += 10;
    CoordY += 30;
    Width -= 15;
  #endif
  
  ttyd::windowdrv::windowDispGX_Waku_col(0, reinterpret_cast<uint8_t *>(&color), CoordX, CoordY, Width, Height, Curve);
  
  // Display the text
  color = 0xFFFFFFFF;
  int32_t PosX = -245;
  int32_t PosY = -35;
  float Scale = 0.9;
  
  #ifdef TTYD_JP
    PosX += 10;
    PosY += 30;
  #endif
  
  sprintf(DisplayBuffer,
    "%s\n%s",
    "Item Randomizers - v1.2.12",
    "Loading Zone Randomizer Beta - v0.5.33");
  
  drawStringMultipleLines(PosX, PosY, color, Scale);
  
  // Display overall version
  PosX = -245;
  PosY = -160;
  
  #ifdef TTYD_JP
    PosY += 10;
  #endif
  
  sprintf(DisplayBuffer,
    "v1.1.34");
  
  drawStringSingleLine(PosX, PosY, color, Scale);
}

void Mod::gameModes()
{
  ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
  ttyd::seqdrv::SeqIndex Load = ttyd::seqdrv::SeqIndex::kLoad;
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
  sprintf(DisplayBuffer,
    "%s\n%s\n%s\n%s",
    "Hold L and press the following to toggle modes:",
    "Y = Item Randomizer v1/v2",
    "X = Loading Zone Randomizer Beta",
    "Z = Loading Zone Randomizer - 1 Hour Challenge");
  
  drawStringMultipleLines(PosX, PosY, color, Scale);
  
  // Set LZRandoText to the appropriate value
  if (LZRando)
  {
    ttyd::string::strcpy(LZRandoText, "On");
  }
  else
  {
    ttyd::string::strcpy(LZRandoText, "Off");
  }
  
  // Set LZRandoChallengeText to the appropriate value
  if (LZRandoChallenge)
  {
    ttyd::string::strcpy(LZRandoChallengeText, "On");
    
    // Set the Item Randomizer to v1 if it's not already
    ItemRandoV2 = false;
  }
  else
  {
    ttyd::string::strcpy(LZRandoChallengeText, "Off");
  }
  
  // Move the text down
  PosY -= 120;
  
  // Display current modes
  sprintf(DisplayBuffer,
    "%s%ld\n%s%s\n%s%s",
    "Item Randomizer: v",
    static_cast<uint32_t>(ItemRandoV2) + 1,
    "Loading Zone Randomizer: ",
    LZRandoText,
    "Loading Zone Randomizer - 1 Hour Challenge: ",
    LZRandoChallengeText);
    
  drawStringMultipleLines(PosX, PosY, color, Scale);
  
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
      
      if (!LZRando)
      {
        // Turn the challenge mode off if it's currently on
        LZRandoChallenge = false;
      }
    }
    DenyInput = true;
  }
  else if ((ButtonInput & LZRandoChallengeButtonCombo) == LZRandoChallengeButtonCombo)
  {
    if (!DenyInput)
    {
      LZRandoChallenge = !LZRandoChallenge;
      
      if (LZRandoChallenge)
      {
        // Turn the Loading Zone randomizer on if it's currently off
        LZRando = true;
      }
    }
    DenyInput = true;
  }
  else
  {
    // Reset flag if no button combo is pressed/held
    DenyInput = false;
  }
}

void Mod::displayStuff()
{
  if (LZRando)
  {
    ttyd::dispdrv::dispEntry(ttyd::dispdrv::DisplayLayer::kDebug3d, 0, [](ttyd::dispdrv::DisplayLayer layerId, void *user)
    {
      reinterpret_cast<Mod *>(user)->LZRandoDisplayStuff();
      
      if (LZRandoChallenge)
      {
        reinterpret_cast<Mod *>(user)->LZRandoChallengeStuff();
      }
    }, this);
  }
  
  ttyd::dispdrv::dispEntry(ttyd::dispdrv::DisplayLayer::k2d, 0, [](ttyd::dispdrv::DisplayLayer layerId, void *user)
  {
    reinterpret_cast<Mod *>(user)->titleScreenStuff();
    reinterpret_cast<Mod *>(user)->gameModes();
  }, this);
}

}