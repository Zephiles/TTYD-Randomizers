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
extern const char *HelpMenuArray[];
extern uint8_t HelpMenuArraySize;

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
  
  uint32_t ButtonInputTrg = ttyd::system::keyGetButtonTrg(0);
  
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
      if (ButtonInputTrg & ShowScoreSourceCombo)
      {
        ShowScoreSources = !ShowScoreSources;
      }
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
        
        const char *TimesUpText = "Time's Up!";
        drawStringSingleLine(PosX, PosY, color, TimesUpText, Scale);
        
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
    "Loading Zone Randomizer Beta - v0.5.50");
  
  drawStringMultipleLines(PosX, PosY, color, DisplayBuffer, Scale);
  
  // Display overall version
  // PosX = -245;
  PosY = -160;
  // Scale = 0.9;
  
  #ifdef TTYD_JP
    PosY += 10;
  #endif
  
  const char *VersionNumber = "v1.1.54";
  drawStringSingleLine(PosX, PosY, color, VersionNumber, Scale);
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
  
  const char *HelpMenuText = "Press L + Start anytime to open the Help menu";
  drawStringSingleLine(PosX, PosY, color, HelpMenuText, Scale);
  
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
  uint32_t ButtonInputTrg = ttyd::system::keyGetButtonTrg(0);
  uint16_t ItemRandoButtonCombo = PAD_L | PAD_Y;
  uint16_t LZRandoButtonCombo = PAD_L | PAD_X;
  uint16_t LZRandoChallengeButtonCombo = PAD_L | PAD_Z;
  
  if ((ButtonInput & ItemRandoButtonCombo) == ItemRandoButtonCombo)
  {
    if (ButtonInputTrg & ItemRandoButtonCombo)
    {
      RandomizeCoins = !RandomizeCoins;
    }
  }
  else if ((ButtonInput & LZRandoButtonCombo) == LZRandoButtonCombo)
  {
    if (ButtonInputTrg & LZRandoButtonCombo)
    {
      LZRando = !LZRando;
      
      if (!LZRando)
      {
        // Turn the challenge mode off if it's currently on
        LZRandoChallenge = false;
      }
    }
  }
  else if ((ButtonInput & LZRandoChallengeButtonCombo) == LZRandoChallengeButtonCombo)
  {
    if (ButtonInputTrg & LZRandoChallengeButtonCombo)
    {
      LZRandoChallenge = !LZRandoChallenge;
      
      if (LZRandoChallenge)
      {
        // Turn the Loading Zone randomizer on if it's currently off
        LZRando = true;
      }
    }
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
  
  // Check for D-Pad inputs
  if ((ButtonInputTrg & PAD_DPAD_LEFT) == PAD_DPAD_LEFT)
  {
    // Go to the previous page
    HelpMenuCounter--;
    
    if (HelpMenuCounter < 1)
    {
      // Loop around to the last page
      HelpMenuCounter = HelpMenuArraySize;
    }
  }
  else if ((ButtonInputTrg & PAD_DPAD_RIGHT) == PAD_DPAD_RIGHT)
  {
    // Go to the next page
    HelpMenuCounter++;
    
    if (HelpMenuCounter > HelpMenuArraySize)
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
    HelpMenuArraySize);
    
  drawStringSingleLine(PosX, PosY, TextColor, DisplayBuffer, Scale);
  
  // Draw the main text
  PosX = -232;
  PosY = 150;
  // Scale = 0.6;
  
  // Get and draw the text for the current page
  const char *PageText = HelpMenuArray[HelpMenuCounter - 1];
  drawStringMultipleLines(PosX, PosY, TextColor, PageText, Scale);
  
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
    // Check if currently on page 5 of the Loading Zone Randomizer section
    if (HelpMenuCounter == (LZRandoStartingPage + 4))
    {
      // Draw second column of Loading Zone Randomizer page 5
      PosX = 0;
      PosY = 110;
      // Scale = 0.6;
      
      const char *BossesText = "15. Shadow Sirens (Ch8)\n16. Grodus\n17. Bowser & Kammy\n18. Shadow Queen\n19. Bonetail";
      drawStringMultipleLines(PosX, PosY, TextColor, BossesText, Scale);
    }
    
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