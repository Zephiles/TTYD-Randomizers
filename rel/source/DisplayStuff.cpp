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
extern uint8_t JustDefeatedBoss;
extern bool BossDefeated[19];
extern uint32_t BossScore;
extern uint32_t GSWAddressesStart;
extern bool InGameOver;
extern uint16_t GameOverCount;
extern uint8_t TimesUpCounter;
extern int32_t FinalScore;
extern bool ShowScoreSources;
extern bool DenyInput;
extern bool NewFile;
extern uint32_t TimerCount;
extern bool TimerDisabled;
extern bool TimerActive;
extern bool DisablePlayerControl;
extern bool GameOverFlag;
extern uint32_t seqMainAddress;
extern bool RandomizeCoins;
extern char *ItemRandoText;
extern bool LZRando;
extern char *LZRandoText;
extern bool LZRandoChallenge;
extern char *LZRandoChallengeText;
extern uint8_t HelpMenuCounter;
extern uint8_t PreviousHelpMenuCounterValue;

namespace mod {

void startDrawString(uint32_t color, float Scale)
{
  ttyd::fontmgr::FontDrawStart();
  ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
  ttyd::fontmgr::FontDrawEdge();
  ttyd::fontmgr::FontDrawScale(Scale);
}

void drawStringSingleLine(int32_t PosX, int32_t PosY, uint32_t color, const char *String, float Scale)
{
  startDrawString(color, Scale);
  ttyd::fontmgr::FontDrawString(PosX, PosY, String);
}

void drawStringMultipleLines(int32_t PosX, int32_t PosY, uint32_t color, const char *String, float Scale)
{
  startDrawString(color, Scale);
  drawstring::drawStringMultiline(PosX, PosY, String);
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
    
  drawStringMultipleLines(PosX, PosY, color, DisplayBuffer, Scale);
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
      // Add 2 points for each level up
      MainScores[4] = (MarioLevel - 1) * 2;
    }
    
    // Check for bosses
    uint32_t BossDefeatedIndex = 0;
    
    // Check for bosses based on current textbox
    for (int i = 1; i <= 9; i++)
    {
      if (JustDefeatedBoss == i)
      {
        if (!BossDefeated[BossDefeatedIndex])
        {
          BossDefeated[BossDefeatedIndex] = true;
          BossScore += 10;
        }
      }
      else
      {
        BossDefeated[BossDefeatedIndex] = false;
      }
      
      BossDefeatedIndex++;
    }
    
    // Check for Grodus and Bowser & Kammy
    for (int i = 10; i <= 11; i++)
    {
      if (JustDefeatedBoss == i)
      {
        if (!BossDefeated[BossDefeatedIndex])
        {
          BossDefeated[BossDefeatedIndex] = true;
          BossScore += 6;
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
    uint16_t SequenceChecks[] = { 85, 200, 211, 260, 388 };
    
    int32_t ArraySize = sizeof(SequenceChecks) / sizeof(SequenceChecks[0]);
    for (int i = 0; i < ArraySize; i++)
    {
      if (SequencePosition == SequenceChecks[i])
      {
        if (!BossDefeated[BossDefeatedIndex])
        {
          BossDefeated[BossDefeatedIndex] = true;
          BossScore += 10;
        }
      }
      else
      {
        BossDefeated[BossDefeatedIndex] = false;
      }
      
      BossDefeatedIndex++;
    }
    
    // Check for the Shadow Queen
    if (SequencePosition == 401)
    {
      if (!BossDefeated[BossDefeatedIndex])
      {
        BossDefeated[BossDefeatedIndex] = true;
        BossScore += 20;
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
      bool AtomicBooCheck = ttyd::swdrv::swGet(2226);
      
      if (AtomicBooCheck && !BossDefeated[BossDefeatedIndex])
      {
        BossDefeated[BossDefeatedIndex] = true;
        BossScore += 10;
      }
    }
    else
    {
      BossDefeated[BossDefeatedIndex] = false;
      
      // Turn off GSWF(2226) so that the player can refight the Atomic Boo
      ttyd::swdrv::swClear(2226);
    }
    
    // Check for Bonetail
    BossDefeatedIndex++;
    if (ttyd::string::strcmp(NextMap, "jon_06") == 0)
    {
      // Check GSWF(5085) - Check the 29 bit
      bool BonetailCheck = ttyd::swdrv::swGet(5085);
      
      if (BonetailCheck && !BossDefeated[BossDefeatedIndex])
      {
        BossDefeated[BossDefeatedIndex] = true;
        BossScore += 20;
      }
    }
    else
    {
      BossDefeated[BossDefeatedIndex] = false;
      
      // Turn off GSWF(5084) and GSWF(5085) so that the player can refight Bonetail
      ttyd::swdrv::swClear(5084);
      ttyd::swdrv::swClear(5085);
    }
    
    // Add 10 points for each boss defeated; 20 points for the Shadow Queen and Bonetail
    MainScores[5] = BossScore;
    
    // Add 1 point for Mario's coin count divided by 100
    int16_t CoinCount = ttyd::mario_pouch::pouchGetCoin();
    MainScores[6] = CoinCount / 100;
    
    // Add additional point for 999 coins
    if (CoinCount >= 999)
    {
      MainScores[6]++;
    }
    
    // Check badge log
    uint32_t GSWAddresses = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
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
    
    // Check for the 3 badges that were not checked in the previous code
    uint32_t CurrentAddress = *reinterpret_cast<uint32_t *>(GSWAddresses + 0x194);
    for (int i = 0; i < 3; i++)
    {
      if (CurrentAddress & (1 << i)) // Check the 0, 1, and 2 bits
      {
        // Add 1 to the count if the bit is on
        BadgeLogCount++;
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
    int32_t MainScoresArraySize = sizeof(MainScores) / sizeof(MainScores[0]);
    for (int i = 0; i < MainScoresArraySize; i++)
    {
      Score += MainScores[i];
    }
    
    if (TimesUpCounter == 1)
    {
      // Update final score
      TimesUpCounter = 2;
      FinalScore = Score;
    }
    
    // Display Score
    int32_t PosX = -232;
    int32_t PosY = -100;
    float Scale = 0.75;
    
    sprintf(DisplayBuffer,
      "Score: %ld",
      Score);
      
    drawStringSingleLine(PosX, PosY, color, DisplayBuffer, Scale);
    
    // Display the Final Score if time is up
    if (TimesUpCounter > 0)
    {
      // PosX = -232;
      PosY = -80;
      // Scale = 0.75;
      
      sprintf(DisplayBuffer,
        "Final Score: %ld",
        FinalScore);
      
      drawStringSingleLine(PosX, PosY, color, DisplayBuffer, Scale);
    }
    
    // Display where the points came from
    if (ShowScoreSources)
    {
      // PosX = -232;
      PosY = -80;
      // Scale = 0.75;
      
      // Move the display up if the Final Score is being displayed
      if (TimesUpCounter > 0)
      {
        PosY += 20;
      }
      
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
      
      drawStringSingleLine(PosX, PosY, color, DisplayBuffer, Scale);
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
    TimerCount = ttyd::system::sysMsec2Frame(3600000); // 1 hour
    ShowScoreSources = false;
    TimerDisabled = false;
    TimerActive = false;
    DisablePlayerControl = false;
    TimesUpCounter = 0;
    BossScore = 0;
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
      
      // Get the proper FPS for the timer
      uint32_t FPS = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
      FPS = *reinterpret_cast<uint32_t *>(FPS + 0x4);
      
      uint32_t hour = TimerCount / 3600 / FPS;
      uint32_t minute = (TimerCount / 60 / FPS) % 60;
      uint32_t second = (TimerCount / FPS) % 60;
      uint32_t frame = TimerCount % FPS;
      
      sprintf(DisplayBuffer,
        "%.2ld:%.2ld:%.2ld.%.2ld",
        hour,
        minute,
        second,
        frame);
      
      drawStringSingleLine(FontDrawX, FontDrawY, color, DisplayBuffer, Scale);
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
        // Update Final Score
        if (TimesUpCounter == 0)
        {
          TimesUpCounter = 1;
          ShowScoreSources = true;
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
        
        const char *String = "Time's Up!";
        drawStringSingleLine(PosX, PosY, color, String, Scale);
        
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
          
          drawStringMultipleLines(PosX, PosY, color, DisplayBuffer, Scale);
          
          // Get input for what to do next
          uint32_t ButtonInputTrg = ttyd::system::keyGetButtonTrg(0);
          
          if ((ButtonInputTrg & PAD_Y) == PAD_Y)
          {
            // Reset the timer to continue playing
            TimerCount = ttyd::system::sysMsec2Frame(3600000); // 1 hour
            ShowScoreSources = false;
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
    "Item Randomizer - v1.2.21",
    "Loading Zone Randomizer Beta - v0.5.48");
  
  drawStringMultipleLines(PosX, PosY, color, DisplayBuffer, Scale);
  
  // Display overall version
  // PosX = -245;
  PosY = -160;
  // Scale = 0.9;
  
  #ifdef TTYD_JP
    PosY += 10;
  #endif
  
  const char *String = "v1.1.52";
  drawStringSingleLine(PosX, PosY, color, String, Scale);
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
  
  // Display text for opening the help menu
  uint32_t color = 0xFFFFFFFF;
  int32_t PosX = -255;
  int32_t PosY = 40;
  float Scale = 0.75;
  
  const char *String = "Press L + Start anytime to open the Help menu";
  drawStringSingleLine(PosX, PosY, color, String, Scale);
  
  // Display game modes
  // PosX = -255;
  PosY = -10;
  // Scale = 0.75;
  
  sprintf(DisplayBuffer,
    "%s\n%s\n%s\n%s",
    "Hold L and press the following to toggle modes:",
    "Y = Randomize Coins",
    "X = Loading Zone Randomizer Beta",
    "Z = Loading Zone Randomizer - 1 Hour Challenge");
  
  drawStringMultipleLines(PosX, PosY, color, DisplayBuffer, Scale);
  
  // Set ItemRandoText to the appropriate value
  if (RandomizeCoins)
  {
    ttyd::string::strcpy(ItemRandoText, "On");
  }
  else
  {
    ttyd::string::strcpy(ItemRandoText, "Off");
  }
  
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
    
    // Don't randomize coins when using the challenge mode
    RandomizeCoins = false;
  }
  else
  {
    ttyd::string::strcpy(LZRandoChallengeText, "Off");
  }
  
  // Move the text down
  // PosX = -255;
  PosY = -120;
  // Scale = 0.75;
  
  // Display current modes
  sprintf(DisplayBuffer,
    "%s%s\n%s%s\n%s%s",
    "Randomize Coins: ",
    ItemRandoText,
    "Loading Zone Randomizer: ",
    LZRandoText,
    "Loading Zone Randomizer - 1 Hour Challenge: ",
    LZRandoChallengeText);
    
  drawStringMultipleLines(PosX, PosY, color, DisplayBuffer, Scale);
  
  // Get input for game modes
  uint32_t ButtonInput = ttyd::system::keyGetButton(0);
  uint16_t ItemRandoButtonCombo = PAD_L | PAD_Y;
  uint16_t LZRandoButtonCombo = PAD_L | PAD_X;
  uint16_t LZRandoChallengeButtonCombo = PAD_L | PAD_Z;
  
  if ((ButtonInput & ItemRandoButtonCombo) == ItemRandoButtonCombo)
  {
    if (!DenyInput)
    {
      RandomizeCoins = !RandomizeCoins;
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

void Mod::helpMenu()
{
  ttyd::seqdrv::SeqIndex Seq = ttyd::seqdrv::seqGetSeq();
  ttyd::seqdrv::SeqIndex Title = ttyd::seqdrv::SeqIndex::kTitle;
  
  // Don't run during the bootup process nor during the intro
  if (Seq < Title)
  {
    return;
  }
  
  // Get input from the player
  uint32_t ButtonInput = ttyd::system::keyGetButton(0);
  uint32_t ButtonInputTrg = ttyd::system::keyGetButtonTrg(0);
  uint16_t OpenMenuCombo = PAD_L | PAD_START;
  
  // Check if the menu should be displayed or not
  if ((ButtonInput & OpenMenuCombo) == OpenMenuCombo)
  {
    if (ButtonInputTrg & OpenMenuCombo)
    {
      if (HelpMenuCounter == 0) // Display the menu
      {
        if (PreviousHelpMenuCounterValue > 0)
        {
          // Set the page to the previous page
          HelpMenuCounter = PreviousHelpMenuCounterValue;
        }
        else
        {
          // Set the page to the first page
          HelpMenuCounter = 1;
        }
      }
      else // Hide the menu
      {
        // Store the current page as the previous page
        PreviousHelpMenuCounterValue = HelpMenuCounter;
        
        // Hide the menu
        HelpMenuCounter = 0;
      }
    }
  }
  
  // Only run if the menu should be displayed
  if (HelpMenuCounter == 0)
  {
    // Menu is not displayed, so exit
    return;
  }
  
  // Set the total number of pages
  uint8_t TotalPages = 24;
  
  // Check for D-Pad inputs
  if ((ButtonInputTrg & PAD_DPAD_LEFT) == PAD_DPAD_LEFT)
  {
    // Go to the previous page
    HelpMenuCounter--;
    
    if (HelpMenuCounter < 1)
    {
      // Loop around to the last page
      HelpMenuCounter = TotalPages;
    }
  }
  else if ((ButtonInputTrg & PAD_DPAD_RIGHT) == PAD_DPAD_RIGHT)
  {
    // Go to the next page
    HelpMenuCounter++;
    
    if (HelpMenuCounter > TotalPages)
    {
      // Loop around to the first page
      HelpMenuCounter = 1;
    }
  }
  
  // Display the window for the text to go in
  uint32_t MenuColor = 0x000000EE;
  float CoordX = -245;
  float CoordY = 190;
  float Width = 490;
  float Height = 375;
  float Curve = 0;
  
  ttyd::windowdrv::windowDispGX_Waku_col(0, reinterpret_cast<uint8_t *>(&MenuColor), CoordX, CoordY, Width, Height, Curve);
  
  // Display the current page
  uint32_t TextColor = 0xFFFFFFFF;
  int32_t PosX = 185;
  int32_t PosY = 180;
  float Scale = 0.6;
  
  sprintf(DisplayBuffer,
    "%d/%d",
    HelpMenuCounter,
    TotalPages);
    
  drawStringSingleLine(PosX, PosY, TextColor, DisplayBuffer, Scale);
  
  // Create char array to use for the pages
  const char *NewPageText;
  
  // Check for which page to display
  switch (HelpMenuCounter)
  {
    case 1: // Basic info
    {
      // Display basic info about the menu
      const char *CurrentPageText = "This menu displays information about the following:\n\n1. Item Randomizer\n   Pages 2-9\n\n2. Loading Zone Randomizer Beta\n   Pages 10-18\n\n3. Loading Zone Randomizer - 1 Hour Challenge\n   Pages 19-24\n\nPress D-Pad Left/Right to change pages\nPress L + Start to close the menu";
      
      NewPageText = CurrentPageText;
      break;
    }   
    case 2: // Item Randomizer page 1
    {
      // Display info about the Item Randomizer
      const char *CurrentPageText = "The following will be randomized:\n\n1. Items/Badges that spawn in the field upon entering\n   a screen\n\n2. Items/Badges from chests and blocks\n\n3. Single Coin Blocks\n\n4. Star Pieces in floor panels\n\n5. Coins from the overworld (only when the Randomize\n   Coins option is On). The coins dropped when\n   running away from a battle are not randomized.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 3: // Item Randomizer page 2
    {
      // Display info about the Item Randomizer
      const char *CurrentPageText = "6. Items held by enemies in battles. All enemies will\n   be holding a random item/badge, and one of these\n   will be dropped at random. Neither the Debug\n   Badge nor important items will ever be\n   held/dropped.\n\n7. Items held by the audience. Any item that spawns\n   in the audience will be randomized (even rocks,\n   bones, etc.), although you will still only get one of\n   the items/badges that is thrown.\n\n8. Standard Shop Items (the Contact Lens and\n   Luigi Books will still appear as normal)\n\n9. Shop Rewards (will never be randomized into the\n   Debug Badge nor important items)";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 4: // Item Randomizer page 3
    {
      // Display info about the Item Randomizer
      const char *CurrentPageText = "10. Hot Dogs bought from Glitzville (will never be\n    randomized into the Debug Badge nor\n    important items)\n\nDue to story progression requirements, the door in\nWestside Goods leading to Don Pianta will always be\nunlocked.\n\nDue to story progression requirements, the Coconuts\nfrom the tree at Keelhaul Key will not be randomized\nduring the time that they are needed.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 5: // Item Randomizer page 4
    {
      // Display info about the Item Randomizer
      const char *CurrentPageText = "In addition to the regular items and badges, the\nfollowing can be obtained:\n\n1. Strange Sack\n2. Crystal Stars (will not advance the Sequence, but\n   will still give you the special move)\n3. Magical Map (gives you Sweet Treat)\n\nWhen the game is about to spawn an item and the\nitem is an important item/upgrade, it first checks to\nsee if you already have that item, and picks a\ndifferent item if you do\n\nThe SP needed for Sweet Feast has been changed\nfrom 5 to 3";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 6: // Item Randomizer page 5
    {
      // Display info about the Item Randomizer
      const char *CurrentPageText = "The SP needed for Showstopper has been changed\nfrom 2 to 4\n\nObtaining a Shine Sprite gives you 3 instead of 1\n\nCollecting the crystal star reward at the end of\nchapters (assuming the item is not a crystal star/\nMagical Map) no longer adjusts your SP, nor does\nit give you a special move\n\nThe following unused items/badges have been added:\n\n1. Koopa Curse - Has the chance to apply the Slow\n   status to all enemies for 3 turns. Icon has been\n   changed to the green Koopa skull.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 7: // Item Randomizer page 6
    {
      // Display info about the Item Randomizer
      const char *CurrentPageText = "2. Cake - Recovers 10 HP\n\n3. Trade Off - Increases all enemies' levels by 5\n   and is stackable.\n\n4. Debug Badge - Automatically does most action\n   commands and superguards. Costs 1 BP to equip.\n   Icon has been changed to the A button.\n\n5. Mega Jump - Jump attack that costs 4 FP and\n   ((Jump Power x 2) + 4) damage to one enemy.\n   Costs 2 BP to equip.\n\n6. Mega Smash - Hammer attack that costs 4 FP and\n   does (Hammer Power + 4) damage to one enemy.\n   Costs 2 BP to equip.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 8: // Item Randomizer page 7
    {
      // Display info about the Item Randomizer
      const char *CurrentPageText = "7. Mega Quake - Hammer attack that costs 7 FP and\n   does 10 damage to all ground enemies. Costs 4 BP\n   to equip\n\n8. Triple Dip - Same as equipping 2 Double Dips.\n   Costs 6 BP to equip.\n\n9. All or Nothing P - All or Nothing for partners\n\n10. FP Drain P - FP Drain for partners\n\n11. Lucky Day P - Lucky Day for partners\n\n12. Lucky Start P - Lucky Start for partners. Uses\n    the same icon as Lucky Start. Costs 2 BP to\n    equip.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 9: // Item Randomizer page 8
    {
      // Display info about the Item Randomizer
      const char *CurrentPageText = "13. Pity Flower P - Pity Flower for partners\n\n14. Super Charge - Boosts attack by +4. Costs 2 BP\n    to equip.\n\n15. Super Charge P - Boosts attack by +4. Costs 2 BP\n    to equip.\n\n16. Art Attack Hitboxes Badge - Displays hitboxes\n    around enemies when using Art Attack. Has the\n    icon of the small Ruby Star. Costs 1 BP to equip.\n\n17. Run Meter Badge - Automatically fills the run\n    meter. Has the icon of the OK image displayed\n    when running away. Costs 1 BP to equip.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 10: // Loading Zone Randomizer page 1
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "This randomizes all of the loading zones. This is\ncurrently still in beta.\n\nStarting from a new file, you will be placed at\nRogueport Square. You are given the Strange Sack,\nall of the partners, the Yoshi color is random, and\nyou start with Yoshi out. After this, any loading zone\nyou enter will be random. Along with this, the\ncurrent sequence position, loading zone used, and\ncurrent map will be displayed on-screen.\n\nThe Sequence (Seq) is where the game thinks you\ncurrently are in the story\n\nThe Loading Zone (LZ) is the entrance that Mario\ncame out of upon entering the room";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 11: // Loading Zone Randomizer page 2
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "The Map is the name of the current room\n\nIf you end up in the Rogueport Sewers room that has\n3 Shine Sprites, you will be able to use the Boat\npanel, regardless of whether you have the Curse or\nnot\n\nIf you end up in the Creepy Steeple Storage Room,\nyou will be able to use the Paper Mode and Tube\nMode curses, regardless of whether you have them or\nnot\n\nIn the event of a Game Over, you can wait for the\nintro to start playing again to resume gameplay on\nthe current file";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 12: // Loading Zone Randomizer page 3
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "Several cheat codes have been added to this mode\nto avoid softlocks:\n\n1. Pressing the following changes the loading zone:\n\n  i.  L + D-Pad Up - changes it to North\n  ii.  L + D-Pad Down - changes it to South\n  iii. L + D-Pad Left - changes it to West\n  iv. L + D-Pad Right - changes it to East\n\n2. L + B reloads the room that you're currently in.\n   There are some instances where you will not be\n   able to reload the room, such as when fighting/\n   about to fight a boss, or being in a Pit room.\n   Reloading the room will also cause the items in the\n   room to respawn.\n\n";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 13: // Loading Zone Randomizer page 4
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "3. L + Z will force Mario off of Yoshi\n\n4. The game will never allow Mario to be transformed\n   into Peach\n\n5. You are able to walk and jump on water. Spin\n   Jumping and going onto water with Yoshi will\n   still cause you to fall into it normally.\n\nWhen warping to a room that has a boss, the\nSequence will be automatically adjusted to allow you\nto fight that boss, assuming that the Sequence was\npreviously below or at the necessary value for that\nboss";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 14: // Loading Zone Randomizer page 5
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "The following Bosses are included:\n\n1. Blooper\n2. Gold Fuzzy\n3. Hooktail\n4. Shadow Sirens (Ch2)\n5. Magnus 1\n6. Grubba\n7. Atomic Boo\n8. Doopliss 1\n9. Doopliss 2\n10. Cortez\n11. Crump (Ch5)\n12. Smorg\n13. Magnus 2\n14. Gloomtail";
      const char *BossesText = "15. Shadow Sirens (Ch8)\n16. Grodus\n17. Bowser & Kammy\n18. Shadow Queen\n19. Bonetail";
      
      // Draw second column
      PosX = 0;
      PosY = 110;
      // Scale = 0.6;
      
      drawStringMultipleLines(PosX, PosY, TextColor, BossesText, Scale);
      
      // Set up first column
      NewPageText = CurrentPageText;
      break;
    }
    case 15: // Loading Zone Randomizer page 6
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "The Escape Timer will never appear in the rooms that\nuse it\n\nThe Sequence will be automatically adjusted to allow\nyou to get the Paper curses, regardless of what value\nit is currently at. The same is true for collecting\nthe Black Keys for them if necessary.\n\nWhen loading into the room with the Ultra Boots, the\nSequence will be automatically adjusted to allow you\nto get them if it's currently above the necessary\nlevel. Additionally, the loading zone will be\nautomatically set if you are currently able to obtain\nthem.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 16: // Loading Zone Randomizer page 7
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "The Cog is automatically inserted upon starting a new\nfile\n\nZess T. is now automatically blocking the west\nentrance upon starting a new file. The Contact Lens\nare also automatically ordered. The Sequence needs\nto be past 55 to be able to buy them, however.\n\nFor the US and EU versions of the game, the Power\nRush and Power Rush P icons will be randomized\nupon starting a new file. They will use the icons from\nthe other versions of the game.\n\nIn the event that you enter a battle with no partner\nout, Goombella will automatically be brought out. The\nexception to this is the Doopliss 2 fight.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 17: // Loading Zone Randomizer page 8
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "When loading an existing file, Mario's coordinates are\nplaced at 0,0,0. This is to prevent spawning out of\nbounds on a map and not being able to get back in\nbounds.\n\nWhen going to fight Doopliss 2, Vivian is forced out\nboth before and after the fight. This is to prevent\nthe game from crashing.\n\nWhen warping to Glitzville, the Sequence will be\nautomatically adjusted for you to be able to get the\nSuper Hammer if you don't have it already. This\nhappens even if you have the Ultra Hammer already.\n\nYou will be warped to the credits after defeating\nthe Shadow Queen.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 18: // Loading Zone Randomizer page 9
    {
      // Display info about the Loading Zone Randomizer
      const char *CurrentPageText = "When warping into either the Glitz Pit Arena or one\nof the locker rooms, any current matches are\nautomatically cleared. This is to prevent softlocks\nas well as a match trying to start at the same time\nas the cutscene for fighting Grubba is playing.\n\nWhen obtaining a follower, it will be randomized into\none of the following:\n\n1. Yoshi Egg\n2. Flavio\n3. Punio\n4. Frankly\n5. Gus";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 19: // 1 Hour Challenge page 1
    {
      // Display info about the 1 Hour Challenge
      const char *CurrentPageText = "The 1 hour challenge is a challenge mode in which\nyou play the game for 1 hour to try to gain as many\npoints as possible. If any of the settings in this\nconflict with the settings of the standard Loading\nZone randomizer, then these settings will take\npriority.\n\nThe points given are as follows:\n\n1. 2 points are given for collecting a crystal star/\n   Magical Map\n\n2. 6 points are given for collecting a curse";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 20: // 1 Hour Challenge page 2
    {
      // Display info about the 1 Hour Challenge
      const char *CurrentPageText = "3. 1 point is given for collecting an important item.\n   The exception to this is the Battle Trunks,\n   which give 1 point per 10 of them.\n\n4. 2 points are given for having a follower. These\n   points are lost if you lose the follower.\n\n5. 2 points are given for each level up\n\n6. 10 points are given for defeating a boss, with the\n   following exceptions:\n\n   i.  20 points are given for the Shadow Queen and\n      Bonetail\n   ii. 6 points are given for Grodus and Bowser &\n      Kammy";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 21: // 1 Hour Challenge page 3
    {
      // Display info about the 1 Hour Challenge
      const char *CurrentPageText = "7. 1 point is given for every 100 coins that you\n   have. An additional point is given for having\n   exactly 999 coins.\n\n8. 1 point is given for every 10 badges in the badge\n   log. This also includes badges that normally do not\n   show up in the log, such as Triple Dip.\n\n9. 5 points are given for every partner upgrade\n\n10. 5 points are lost for every Game Over that occurs";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 22: // 1 Hour Challenge page 4
    {
      // Display info about the 1 Hour Challenge
      const char *CurrentPageText = "The current Score and Time are displayed on-screen.\nWhen the timer reaches 0, the game will state that\nthe time is up, and will give the options to either\ncontinue playing or to return to the title screen. The\nFinal Score will also be displayed, as well as where\neach of the points came from. The order of the\npoints are the same as the list prior. If you choose\nto continue playing, then the timer will reset to 1\nhour, but the Final Score will remain displayed. Both\nthe Score and the sources for the Score will still\nupdate, but the Final Score will remain unchanged.\n\nL + R can be pressed at anytime during gameplay to\ntoggle the display for where the points came from\n\nMost cutscenes are skipped or shortened";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 23: // 1 Hour Challenge page 5
    {
      // Display info about the 1 Hour Challenge
      const char *CurrentPageText = "Many maps from the standard Loading Zone\nrandomizer are excluded\n\nThe Debug Badge will never spawn\n\nCrystal stars and/or the Magical Map cannot be\nobtained within the first 5 minutes\n\nThe Pit cannot be accessed within the first 10 minutes\n\nBosses cannot be accessed within the first 20 minutes\n\nSaying No to the Shadow Queen will warp you to a\ndifferent room instead of resulting in a Game Over.\nHowever, doing so will set the Sequence to 405.";
      
      NewPageText = CurrentPageText;
      break;
    }
    case 24: // 1 Hour Challenge page 6
    {
      // Display info about the 1 Hour Challenge
      const char *CurrentPageText = "Defeating the Shadow Queen will warp you to a\ndifferent room\n\nWhen a boss that does not require a specific\nSequence (such as the Atomic Boo) is defeated, you\nmust go to a different room in order for them to\nrespawn. Warping to the same room will not respawn\nthem; you must warp to a different room first.";
      
      NewPageText = CurrentPageText;
      break;
    }
    default:
    {
      // Something went wrong, so exit the function
      return;
    }
  }
  
  // Draw the main text
  PosX = -232;
  PosY = 150;
  // Scale = 0.6;
  
  drawStringMultipleLines(PosX, PosY, TextColor, NewPageText, Scale);
  
  // Display page titles
  uint32_t ItemRandoStartingPage = 2;
  uint32_t ItemRandoTotalPages = 8;
  uint32_t LZRandoStartingPage = ItemRandoStartingPage + ItemRandoTotalPages;
  uint32_t LZRandoTotalPages = 9;
  uint32_t ChallengeModeStartingPage = LZRandoStartingPage + LZRandoTotalPages;
  uint32_t ChallengeModeTotalPages = 6;
  
  if ((HelpMenuCounter >= ItemRandoStartingPage) && (HelpMenuCounter <= (ItemRandoStartingPage + ItemRandoTotalPages - 1)))
  {
    // Draw the Item Randomizer page title
    PosX = -80;
    PosY = 180;
    // Scale = 0.6;
    
    sprintf(DisplayBuffer,
      "%s (%ld)",
      "Item Randomizer",
      HelpMenuCounter - ItemRandoStartingPage + 1);
  }
  else if ((HelpMenuCounter >= LZRandoStartingPage) && (HelpMenuCounter <= (LZRandoStartingPage + LZRandoTotalPages - 1)))
  {
    // Draw the Loading Zone Randomizer page title
    PosX = -125;
    PosY = 180;
    // Scale = 0.6;
    
    sprintf(DisplayBuffer,
      "%s (%ld)",
      "Loading Zone Randomizer",
      HelpMenuCounter - LZRandoStartingPage + 1);
  }
  else if ((HelpMenuCounter >= ChallengeModeStartingPage) && (HelpMenuCounter <= (ChallengeModeStartingPage + ChallengeModeTotalPages - 1)))
  {
    // Draw the 1 Hour Challenge page title
    PosX = -80;
    PosY = 180;
    // Scale = 0.6;
    
    sprintf(DisplayBuffer,
      "%s (%ld)",
      "1 Hour Challenge",
      HelpMenuCounter - ChallengeModeStartingPage + 1);
  }
  else
  {
    // Exit the function if there is no more text to draw
    return;
  }
  
  drawStringSingleLine(PosX, PosY, TextColor, DisplayBuffer, Scale);
}

void Mod::displayStuff()
{
  ttyd::dispdrv::dispEntry(ttyd::dispdrv::DisplayLayer::kDebug3d, 0, [](ttyd::dispdrv::DisplayLayer layerId, void *user)
  {
    if (LZRando)
    {
      reinterpret_cast<Mod *>(user)->LZRandoDisplayStuff();
    
      if (LZRandoChallenge)
      {
        reinterpret_cast<Mod *>(user)->LZRandoChallengeStuff();
      }
    }
    
    reinterpret_cast<Mod *>(user)->helpMenu();
  }, this);
  
  ttyd::dispdrv::dispEntry(ttyd::dispdrv::DisplayLayer::k2d, 0, [](ttyd::dispdrv::DisplayLayer layerId, void *user)
  {
    reinterpret_cast<Mod *>(user)->titleScreenStuff();
    reinterpret_cast<Mod *>(user)->gameModes();
  }, this);
}

}