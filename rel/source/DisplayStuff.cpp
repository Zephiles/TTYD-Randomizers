#include "mod.h"
#include "drawstring.h"
#include "managestrings.h"
#include "items.h"
#include "buttons.h"

#include <ttyd/icondrv.h>
#include <ttyd/fontmgr.h>
#include <ttyd/windowdrv.h>
#include <ttyd/seqdrv.h>
#include <ttyd/swdrv.h>
#include <ttyd/system.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/mario_party.h>
#include <ttyd/__mem.h>
#include <ttyd/mariost.h>
#include <ttyd/mario.h>
#include <ttyd/dispdrv.h>

#include <cstdint>
#include <cstdio>
#include <cinttypes>

extern char *NextMap;
extern bool TransparentTextFlag;
extern char *DisplayBuffer;
extern char *NextBero;
extern int32_t *AllFinalScoresArray;
extern uint32_t TotalFinalScoresCounter;
extern bool FinishedARun;
extern uint8_t FinishedRunCounter;
extern int32_t FinalScore;
extern bool NewFile;
extern bool GameOverChallengeMode;
extern uint32_t seqMainAddress;
extern uint32_t FinalScoresMenuCounter;
extern uint8_t JustDefeatedBoss;
extern bool BossDefeated[22];
extern uint32_t BossScore;
extern uint32_t GSWAddressesStart;
extern bool InGameOver;
extern uint16_t GameOverCount;
extern uint8_t TimesUpCounter;
extern bool ShowScoreSources;
extern uint32_t TimerCount;
extern bool TimerDisabled;
extern bool TimerActive;
extern bool DisablePlayerControl;
extern bool GameOverFlag;
extern uint32_t TitleWorkPointer2;
extern bool RandomizeCoins;
extern bool LZRando;
extern bool LZRandoChallenge;
extern uint8_t HelpMenuCounter;
extern uint8_t PreviousHelpMenuCounterValue;
extern const char *HelpMenuArray[];
extern uint8_t HelpMenuArraySize;

namespace mod
{
    int32_t *drawIcon(int32_t position[3], uint16_t iconNum, float scale)
    {
        float NewPosition[3];
        int32_t ArraySize = static_cast<int32_t>(sizeof(NewPosition) / sizeof(float));

        for (int32_t i = 0; i < ArraySize; i++)
        {
            NewPosition[i] = static_cast<float>(position[i]);
        }

        ttyd::icondrv::iconDispGx(NewPosition, 24, iconNum, scale);
        return position;
    }

    int32_t *drawIconFromItem(int32_t position[3], uint16_t itemNum, float scale)
    {
#ifdef TTYD_US
        uint32_t itemDataTable = 0x803108A8;
#elif defined TTYD_JP
        uint32_t itemDataTable = 0x8030EE58;
#elif defined TTYD_EU
        uint32_t itemDataTable = 0x8031C638;
#endif

        uint16_t iconNum = *reinterpret_cast<uint16_t *>(itemDataTable + (itemNum * 0x28) + 0x20);
        return drawIcon(position, iconNum, scale);
    }

    void drawWindow(uint32_t color, int32_t x, int32_t y, int32_t width, int32_t height, int32_t curve)
    {
        ttyd::windowdrv::windowDispGX_Waku_col(0, reinterpret_cast<uint8_t *>(&color), x, y, width, height, curve);
    }

    void drawMenuWindow()
    {
        uint32_t color = 0x000000F4;
        int32_t x = -245;
        int32_t y = 190;
        int32_t width = 490;
        int32_t height = 375;
        int32_t curve = 0;

        drawWindow(color, x, y, width, height, curve);
    }

    void startDrawString(uint8_t alpha, uint32_t color, float scale)
    {
        ttyd::fontmgr::FontDrawStart_alpha(alpha);
        ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
        ttyd::fontmgr::FontDrawEdge();
        ttyd::fontmgr::FontDrawScale(scale);
    }

    void drawText(const char *text, int32_t x, int32_t y, uint8_t alpha, uint32_t color, float scale)
    {
        startDrawString(alpha, color, scale);
        bool MultipleLines = false;

        uint32_t i = 0;
        bool LoopDone = false;
        while (!LoopDone)
        {
            if (text[i] == '\n')
            {
                MultipleLines = true;
                LoopDone = true;
            }
            else if (text[i] == '\0')
            {
                LoopDone = true;
            }
            i++;
        }

        float NewX = static_cast<float>(x);
        float NewY = static_cast<float>(y);

        if (MultipleLines)
        {
            // The text has multiple lines
            drawstring::drawStringMultiline(NewX, NewY, text);
        }
        else
        {
            // The text has one line
            ttyd::fontmgr::FontDrawString(NewX, NewY, text);
        }
    }

    void drawTextWithWindow(const char *text,
                            int32_t textPosX,
                            int32_t textPosY,
                            uint8_t alpha,
                            uint32_t textColor,
                            float textScale,
                            int32_t windowWidth,
                            uint32_t windowColor,
                            float windowCurve)
    {
        // Check if the text is one line or multiple lines
        uint32_t AdditionalLines = 0;
        uint32_t i = 0;

        bool LoopDone = false;
        while (!LoopDone)
        {
            if (text[i] == '\n')
            {
                AdditionalLines++;
            }
            else if (text[i] == '\0')
            {
                LoopDone = true;
            }
            i++;
        }

        // Get the Height, X, and Y for the window
        int32_t WindowHeight = (AdditionalLines * 20) + 44;
        int32_t WindowPosX = textPosX - 15;
        int32_t WindowPosY = textPosY + 10;

        // Draw the window
        drawWindow(windowColor, WindowPosX, WindowPosY, windowWidth, WindowHeight, windowCurve);

        // Draw the text
        drawText(text, textPosX, textPosY, alpha, textColor, textScale);
    }

    void Mod::LZRandoDisplayStuff()
    {
        ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
        ttyd::seqdrv::SeqIndex Game = ttyd::seqdrv::SeqIndex::kGame;
        ttyd::seqdrv::SeqIndex GameOver = ttyd::seqdrv::SeqIndex::kGameOver;

        bool dmo_comparison = managestrings::strcmp_NextMap("dmo_00");
        bool title_comparison = managestrings::strcmp_NextMap("title");

        // Only display while a file is loaded, and while not in battles
        // Don't display while in the intro or on the title screen
        if ((NextSeq < Game) || (NextSeq > GameOver) || dmo_comparison || title_comparison)
        {
            return;
        }

        // Check for user input
        uint32_t ButtonInput = ttyd::system::keyGetButton(0);
        uint16_t TransparentTextCombo = PAD_Z | PAD_DPAD_UP;
        if ((ButtonInput & TransparentTextCombo) == TransparentTextCombo)
        {
            uint32_t ButtonInputTrg = ttyd::system::keyGetButtonTrg(0);
            if (ButtonInputTrg & TransparentTextCombo)
            {
                TransparentTextFlag = !TransparentTextFlag;
            }
        }

        uint32_t color = 0xFFFFFFFF;
        int32_t PosX = -232;
        int32_t PosY = -120;
        float Scale = 0.75;
        char *tempDisplayBuffer = DisplayBuffer; // Prevent DisplayBuffer from being loaded in multiple times

        sprintf(tempDisplayBuffer, "Seq: %" PRIu32 "\nLZ: %s\nMap: %s", ttyd::swdrv::swByteGet(0), NextBero, NextMap);

        uint8_t alpha;
        if (!TransparentTextFlag)
        {
            alpha = 0xFF;
        }
        else
        {
            alpha = 0x50;
        }

        drawText(tempDisplayBuffer, PosX, PosY, alpha, color, Scale);
    }

    void adjustAllFinalScoresArray()
    {
        // Initialize array if not already initialized
        if (AllFinalScoresArray == nullptr)
        {
            AllFinalScoresArray = new int32_t[1];
        }
        else
        {
            // Create new array with a new spot for the newewst Final Score
            uint32_t Size = TotalFinalScoresCounter;
            int32_t *tempFinalScoresArray = new int32_t[Size + 1];

            // Copy the contents of the old array to the new array
            ttyd::system::memcpy_as4(tempFinalScoresArray, AllFinalScoresArray, (Size * sizeof(int32_t)));

            // Delete the old array
            delete[] (AllFinalScoresArray);

            // Set the new array as the current array
            AllFinalScoresArray = tempFinalScoresArray;
        }

        TotalFinalScoresCounter++;
    }

    void displayAllFinalScores()
    {
        if (FinishedARun)
        {
            // Add the newest Final Score to the array
            FinishedARun = false;

            if (FinishedRunCounter != 2)
            {
                adjustAllFinalScoresArray();
                AllFinalScoresArray[TotalFinalScoresCounter - 1] = FinalScore;
            }

            // Prevent new scores from being added if the player chose to continue playing instead of warping back to the title
            // screen
            if (FinishedRunCounter == 1)
            {
                FinishedRunCounter = 2;
            }
        }
        else if (NewFile && GameOverChallengeMode)
        {
            // The current run was reset, so write 0x80000000
            adjustAllFinalScoresArray();
            AllFinalScoresArray[TotalFinalScoresCounter - 1] = static_cast<int32_t>(0x80000000);
        }

        ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
        ttyd::seqdrv::SeqIndex Load = ttyd::seqdrv::SeqIndex::kLoad;
        uint32_t seqMainCheck = *reinterpret_cast<uint32_t *>(seqMainAddress + 0x4);

        bool Comparisons = (NextSeq == Load) && (seqMainCheck == 4);

        // Only run on the file select screen
        if (!Comparisons)
        {
            // Turn the menu off it's still on
            FinalScoresMenuCounter = 0;

            return;
        }

        // Get input from the player
        uint32_t ButtonInput = ttyd::system::keyGetButton(0);
        uint32_t ButtonInputTrg = ttyd::system::keyGetButtonTrg(0);
        uint16_t OpenFinalScoreListCombo = PAD_L | PAD_Y;

        // Display/Remove the menu
        if ((ButtonInput & OpenFinalScoreListCombo) == OpenFinalScoreListCombo)
        {
            if (ButtonInputTrg & OpenFinalScoreListCombo)
            {
                if (FinalScoresMenuCounter == 0)
                {
                    // Turn the menu on and set it to the first page
                    FinalScoresMenuCounter = 1;
                }
                else
                {
                    // Turn the menu off
                    FinalScoresMenuCounter = 0;
                }
            }
        }

        // Only run if the menu should be displayed
        if (FinalScoresMenuCounter == 0)
        {
            // Menu is not displayed, so exit
            return;
        }

        // Set total amount of scores to display in one column
        uint32_t TotalInColumn = 16;

        // Set the total amount of columns on one page
        uint32_t TotalColumns = 4;

        // Set total amount of scores to display on one page
        uint32_t TotalOnPage = TotalColumns * TotalInColumn;

        // Prevent TotalFinalScoresCounter from being loaded multiple times
        uint32_t FinalScoresCounter = TotalFinalScoresCounter;

        // Get the total amount of pages
        uint32_t TotalPages;
        if (FinalScoresCounter > 0)
        {
            TotalPages = 1 + ((FinalScoresCounter - 1) / TotalOnPage); // Round up
        }
        else
        {
            TotalPages = 1;
        }

        // Check for D-Pad inputs
        if ((ButtonInputTrg & PAD_DPAD_LEFT) == PAD_DPAD_LEFT)
        {
            // Go to the previous page
            FinalScoresMenuCounter--;

            if (FinalScoresMenuCounter < 1)
            {
                // Loop around to the last page
                FinalScoresMenuCounter = TotalPages;
            }
        }
        else if ((ButtonInputTrg & PAD_DPAD_RIGHT) == PAD_DPAD_RIGHT)
        {
            // Go to the next page
            FinalScoresMenuCounter++;

            if (FinalScoresMenuCounter > TotalPages)
            {
                // Loop around to the first page
                FinalScoresMenuCounter = 1;
            }
        }

        // Display the window for the text to go in
        drawMenuWindow();

        // Display the current page
        uint8_t alpha = 0xFF;
        uint32_t TextColor = 0xFFFFFFFF;
        int32_t PosX = 185;
        int32_t PosY = 180;
        float Scale = 0.6;
        char *tempDisplayBuffer = DisplayBuffer; // Prevent DisplayBuffer from being loaded in multiple times

        sprintf(tempDisplayBuffer, "%" PRId32 "/%" PRId32, FinalScoresMenuCounter, TotalPages);

        drawText(tempDisplayBuffer, PosX, PosY, alpha, TextColor, Scale);

        // Draw the page title
        PosX = -80;
        PosY = 180;
        // Scale = 0.6;

        const char *TitleText = "Final Scores List";
        drawText(TitleText, PosX, PosY, alpha, TextColor, Scale);

        // Don't draw anything else if there are no scores in the array
        if (AllFinalScoresArray == nullptr)
        {
            // Array is empty, so exit
            return;
        }

        // Draw the main page text
        PosX = -232;
        PosY = 150;
        // Scale = 0.6;

        // Get the starting index
        uint32_t StartingIndex = (FinalScoresMenuCounter - 1) * TotalOnPage;

        // Draw the current page
        for (uint32_t j = 0; j < TotalColumns; j++)
        {
            // Draw the current column
            uint32_t CurrentStartingIndex = StartingIndex + (j * TotalInColumn);
            for (uint32_t i = CurrentStartingIndex; i < (CurrentStartingIndex + TotalInColumn); i++)
            {
                // Stop drawing if at the end of the array
                if (i >= FinalScoresCounter)
                {
                    // No other code has to run, so exit
                    return;
                }

                // Get the current line to draw
                if (AllFinalScoresArray[i] != static_cast<int32_t>(0x80000000))
                {
                    // The player did not reset this run
                    sprintf(tempDisplayBuffer, "%" PRId32 ". %" PRId32, i + 1, AllFinalScoresArray[i]);
                }
                else
                {
                    // The player reset this run
                    sprintf(tempDisplayBuffer, "%" PRId32 ". Reset", i + 1);
                }

                // Draw the current line
                drawText(tempDisplayBuffer, PosX, PosY, alpha, TextColor, Scale);

                // Implement a new line
                PosY -= 20;
            }

            // Implement the next column
            PosX += 115;
            PosY = 150;
        }
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

        uint8_t alpha;
        if (!TransparentTextFlag)
        {
            alpha = 0xFF;
        }
        else
        {
            alpha = 0x50;
        }

        bool dmo_comparison = managestrings::strcmp_NextMap("dmo_00");
        bool title_comparison = managestrings::strcmp_NextMap("title");

        uint32_t ButtonInputTrg = ttyd::system::keyGetButtonTrg(0);
        char *tempDisplayBuffer = DisplayBuffer; // Prevent DisplayBuffer from being loaded in multiple times

        // Get and display Score
        // Don't display while in the intro or on the title screen
        if ((NextSeq >= Game) && (NextSeq <= GameOver) && !dmo_comparison && !title_comparison)
        {
            int32_t Score = 0;
            int32_t MainScores[11] = {0};
            uint32_t PouchPointer = reinterpret_cast<uint32_t>(ttyd::mario_pouch::pouchGetPtr());

            // Check Important Items
            uint32_t ImportantItemsAddressesStart = PouchPointer + 0xA0;
            for (int i = 0; i < 121; i++)
            {
                uint16_t ImportantItem = *reinterpret_cast<uint16_t *>(ImportantItemsAddressesStart + (i * 0x2));
                if ((ImportantItem != Hammer) && (ImportantItem != MagicalMap) && (ImportantItem != Boots) &&
                    (ImportantItem != MailboxSP) && (ImportantItem != StrangeSack) && (ImportantItem != BattleTrunks))
                {
                    if ((ImportantItem == MagicalMapBigger) ||
                        ((ImportantItem >= DiamondStar) && (ImportantItem <= CrystalStar)))
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
            uint32_t FollowerPointer =
                reinterpret_cast<uint32_t>(ttyd::party::partyGetPtr(ttyd::mario_party::marioGetExtraPartyId()));
            if (FollowerPointer)
            {
                // Add 2 points if the player has a follower
                MainScores[3] = 2;
            }
            else
            {
                ttyd::mario::Player *player = ttyd::mario::marioGetPtr();

#ifdef TTYD_JP
                // JP has the uint8_t in a different location?
                uint8_t prevFollower = player->unk_244;
#else
                uint8_t prevFollower = player->prevFollowerId[1];
#endif

                if (prevFollower > 0)
                {
                    // Add 2 points if the player has a follower
                    MainScores[3] = 2;
                }
            }

            // Check for level ups
            int16_t MarioLevel = *reinterpret_cast<int16_t *>(PouchPointer + 0x8A);
            if (MarioLevel > 1)
            {
                // Add 2 points for each level up
                MainScores[4] = (MarioLevel - 1) * 2;
            }

            // Check for bosses
            uint32_t TotalBosses = sizeof(BossDefeated) / sizeof(BossDefeated[0]);
            uint32_t TotalBossesTextbox10Points = 10;
            uint32_t StartSequenceChecks = TotalBossesTextbox10Points + 3; // 2 used for Grodus and Bowser & Kammy
            uint32_t EndSequenceChecks = StartSequenceChecks + 6;          // 7 Total
            uint32_t ShadowQueenNumCheck = EndSequenceChecks + 1;
            uint32_t AtomicBooNumCheck = EndSequenceChecks + 2;
            uint32_t BonetailNumCheck = EndSequenceChecks + 3;

            uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
            static const uint16_t SequenceChecks[] = {39, 85, 200, 211, 260, 383, 388};

            for (uint32_t i = 1; i <= TotalBosses; i++)
            {
                // Check for bosses based on current textbox
                if (i <= TotalBossesTextbox10Points)
                {
                    if (i == JustDefeatedBoss)
                    {
                        if (!BossDefeated[i - 1])
                        {
                            BossDefeated[i - 1] = true;
                            BossScore += 10;
                        }
                    }
                    else
                    {
                        BossDefeated[i - 1] = false;
                    }
                }
                else if ((i == (TotalBossesTextbox10Points + 1)) || (i == (TotalBossesTextbox10Points + 2)))
                {
                    // Check for Grodus and Bowser & Kammy
                    if (i == JustDefeatedBoss)
                    {
                        if (!BossDefeated[i - 1])
                        {
                            BossDefeated[i - 1] = true;
                            BossScore += 9;
                        }
                    }
                    else
                    {
                        BossDefeated[i - 1] = false;
                    }
                }
                else if ((i >= StartSequenceChecks) && (i <= EndSequenceChecks))
                {
                    // Check for bosses based on Sequence
                    if (SequencePosition == SequenceChecks[i - StartSequenceChecks])
                    {
                        if (!BossDefeated[i - 1])
                        {
                            BossDefeated[i - 1] = true;
                            BossScore += 10;
                        }
                    }
                    else
                    {
                        BossDefeated[i - 1] = false;
                    }
                }
                else if (i == ShadowQueenNumCheck)
                {
                    // Check for the Shadow Queen
                    if (SequencePosition == 401)
                    {
                        if (!BossDefeated[i - 1])
                        {
                            BossDefeated[i - 1] = true;
                            BossScore += 30;
                        }
                    }
                    else
                    {
                        BossDefeated[i - 1] = false;
                    }
                }
                else if (i == AtomicBooNumCheck)
                {
                    // Check for the Atomic Boo
                    if (managestrings::strcmp_NextMap("jin_00"))
                    {
                        // Check GSWF(2226)
                        bool AtomicBooCheck = ttyd::swdrv::swGet(2226);

                        if (AtomicBooCheck && !BossDefeated[i - 1])
                        {
                            BossDefeated[i - 1] = true;
                            BossScore += 10;
                        }
                    }
                    else
                    {
                        BossDefeated[i - 1] = false;

                        // Turn off GSWF(2226) so that the player can refight the Atomic Boo
                        ttyd::swdrv::swClear(2226);
                    }
                }
                else if (i == BonetailNumCheck)
                {
                    // Check for Bonetail
                    if (managestrings::strcmp_NextMap("jon_06"))
                    {
                        // Check GSWF(5085)
                        bool BonetailCheck = ttyd::swdrv::swGet(5085);

                        if (BonetailCheck && !BossDefeated[i - 1])
                        {
                            BossDefeated[i - 1] = true;
                            BossScore += 30;
                        }
                    }
                    else
                    {
                        BossDefeated[i - 1] = false;
                    }
                }
            }

            // Add 10 points for each boss defeated; 25 points for the Shadow Queens and 30 points for Bonetail
            MainScores[5] = BossScore;

            // Add 1 point for every 100 coins that the player has
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

            // Add 1 point for every Star Piece the player has
            MainScores[9] = ttyd::mario_pouch::pouchGetStarPiece();

            // Subtract 5 points for each Game Over
            MainScores[10] -= GameOverCount * 5;

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

            sprintf(tempDisplayBuffer, "Score: %" PRId32 "", Score);

            drawText(tempDisplayBuffer, PosX, PosY, alpha, color, Scale);

            // Display the Final Score if time is up
            if (TimesUpCounter > 0)
            {
                // PosX = -232;
                PosY = -80;
                // Scale = 0.75;

                sprintf(tempDisplayBuffer, "Final Score: %" PRId32, FinalScore);

                drawText(tempDisplayBuffer, PosX, PosY, alpha, color, Scale);
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

                sprintf(tempDisplayBuffer,
                        "%" PRId32 " %" PRId32 " %" PRId32 " %" PRId32 " %" PRId32 " %" PRId32 " %" PRId32 " %" PRId32
                        " %" PRId32 " %" PRId32 " %" PRId32,
                        MainScores[0],   // Crystal Stars/Magical Map
                        MainScores[1],   // Curses
                        MainScores[2],   // Important Items
                        MainScores[3],   // Follower
                        MainScores[4],   // Level Ups
                        MainScores[5],   // Bosses
                        MainScores[6],   // Coins
                        MainScores[7],   // Badge Log
                        MainScores[8],   // Partner Upgrades
                        MainScores[9],   // Star Pieces
                        MainScores[10]); // Game Overs

                drawText(tempDisplayBuffer, PosX, PosY, alpha, color, Scale);
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
            FinishedRunCounter = 0;
            JustDefeatedBoss = 0;
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

                sprintf(tempDisplayBuffer, "%02" PRIu32 ":%02" PRIu32 ":%02" PRIu32 ".%02" PRIu32, hour, minute, second, frame);

                drawText(tempDisplayBuffer, FontDrawX, FontDrawY, alpha, color, Scale);
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
                        TransparentTextFlag = false;
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
                    drawText(TimesUpText, PosX, PosY, alpha, color, Scale);

                    // // Don't display or allow modifying of the options in battles, as trying to warp as the curtain is coming
                    // up will crash the game
                    if (NextSeq != Battle)
                    {
                        // Display text for how the player wants to continue
                        PosX = -225;
                        PosY = 25;
                        Scale = 0.9;

                        sprintf(tempDisplayBuffer,
                                "%s\n%s",
                                "Press Y to continue playing",
                                "Press X to return to the title screen");

                        drawText(tempDisplayBuffer, PosX, PosY, alpha, color, Scale);

                        // Get input for what to do next
                        if ((ButtonInputTrg & PAD_Y) == PAD_Y)
                        {
                            // Reset the timer to continue playing
                            TimerCount = ttyd::system::sysMsec2Frame(3600000); // 1 hour
                            ShowScoreSources = false;
                            DisablePlayerControl = false;
                            ttyd::mariost::marioStSystemLevel(0);
                            FinishedARun = true;

                            if (FinishedRunCounter == 0)
                            {
                                FinishedRunCounter = 1;
                            }

                            // Only turn the key on if it's not already on
                            if (ttyd::mario::marioKeyOffChk() != 0)
                            {
                                ttyd::mario::marioKeyOn();
                            }
                        }
                        else if ((ButtonInputTrg & PAD_X) == PAD_X)
                        {
                            // Return to the title screen
                            FinishedARun = true;
                            TimerDisabled = true;
                            GameOverFlag = false;
                            GameOverChallengeMode = false;
                            ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, "title", nullptr);
                            ttyd::mariost::marioStSystemLevel(0);
                        }
                    }
                }
            }
        }

        // Handle the menu for all of the final scores
        displayAllFinalScores();
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
        uint8_t alpha = 0xFF;
        uint32_t TextColor = 0xFFFFFFFF;
        int32_t PosX = -203;
        int32_t PosY = -35;
        float Scale = 0.9;
        int32_t WindowWidth = 457;
        uint32_t WindowColor = 0x000000CC;
        int32_t Curve = 10;
        char *tempDisplayBuffer = DisplayBuffer; // Prevent DisplayBuffer from being loaded in multiple times

#ifdef TTYD_JP
        PosX += 5;
        PosY += 30;
        WindowWidth -= 10;
#endif

        sprintf(tempDisplayBuffer, "%s\n%s", "Item Randomizer - v1.3.6", "Loading Zone Randomizer - v1.0.32");

        drawTextWithWindow(tempDisplayBuffer, PosX, PosY, alpha, TextColor, Scale, WindowWidth, WindowColor, Curve);

        // Display overall version
        PosX = -230;
        PosY = -160;
        // Scale = 0.9;

#ifdef TTYD_JP
        PosX += 15;
        PosY += 15;
#endif

        const char *VersionNumber = "v2.0.34";
        drawText(VersionNumber, PosX, PosY, alpha, TextColor, Scale);

        // Draw the remaining time for when gameplay will resume
        if (GameOverFlag && LZRando)
        {
            // The text should only be displayed if the player gets a Game Over and is using the Loading Zone randomizer
            // alpha = 0xFF;
            // TextColor = 0xFFFFFFFF;
            PosX = -183;
            PosY = 55;
            // Scale = 0.9;
            WindowWidth = 404;
            WindowColor = 0x000000F4;
            // Curve = 10;

#ifdef TTYD_JP
            PosX += 5;
            PosY += 30;
            WindowWidth -= 11;
#endif

            uint32_t TotalWaitTime = ttyd::system::sysMsec2Frame(7000); // 7 seconds
            uint32_t tempTitleWorkPointer2 = *reinterpret_cast<uint32_t *>(TitleWorkPointer2);
            uint32_t CurrentWaitTime = *reinterpret_cast<uint32_t *>(tempTitleWorkPointer2 + 0x28);
            int32_t RemainingTime = TotalWaitTime - CurrentWaitTime;

            // CurrentWaitTime will always end up being 1 frame over TotalWaitTime, so prevent that from displaying
            if (RemainingTime < 0)
            {
                RemainingTime = 0;
            }

            // Get the proper FPS for the timer
            uint32_t FPS = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
            FPS = *reinterpret_cast<uint32_t *>(FPS + 0x4);

            uint32_t second = (RemainingTime / FPS) % 60;
            uint32_t frame = RemainingTime % FPS;

            sprintf(tempDisplayBuffer, "Gameplay will resume in: %02" PRIu32 ":%02" PRIu32, second, frame);

            drawTextWithWindow(tempDisplayBuffer, PosX, PosY, alpha, TextColor, Scale, WindowWidth, WindowColor, Curve);
        }
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
        uint8_t alpha = 0xFF;
        uint32_t color = 0xFFFFFFFF;
        int32_t PosX = -255;
        int32_t PosY = 40;
        float Scale = 0.75;

        const char *HelpMenuText = "Press L + Start anytime to open the Help menu";
        drawText(HelpMenuText, PosX, PosY, alpha, color, Scale);

        // Display game modes
        // PosX = -255;
        PosY = -10;
        // Scale = 0.75;
        char *tempDisplayBuffer = DisplayBuffer; // Prevent DisplayBuffer from being loaded in multiple times

        sprintf(tempDisplayBuffer,
                "%s\n%s\n%s\n%s",
                "Hold L and press the following to toggle modes:",
                "Y = Randomize Coins",
                "X = Loading Zone Randomizer",
                "Z = Loading Zone Randomizer - 1 Hour Challenge");

        drawText(tempDisplayBuffer, PosX, PosY, alpha, color, Scale);

        // Display the Item Randomizer setting as On or Off
        char ItemRandoText[4]; // 3 bytes for ItemRandoText, 1 byte for NULL
        if (RandomizeCoins)
        {
            managestrings::strcpy_String(ItemRandoText, "On");
        }
        else
        {
            managestrings::strcpy_String(ItemRandoText, "Off");
        }

        // Display the Loading Zone Randomizer setting as On or Off
        char LZRandoText[4]; // 3 bytes for LZRandoText, 1 byte for NULL
        if (LZRando)
        {
            managestrings::strcpy_String(LZRandoText, "On");
        }
        else
        {
            managestrings::strcpy_String(LZRandoText, "Off");
        }

        // Display the Challenge Mode setting as On or Off
        char LZRandoChallengeText[4]; // 3 bytes for LZRandoChallengeText, 1 byte for NULL
        if (LZRandoChallenge)
        {
            managestrings::strcpy_String(LZRandoChallengeText, "On");

            // Don't randomize coins when using the challenge mode
            RandomizeCoins = false;
        }
        else
        {
            managestrings::strcpy_String(LZRandoChallengeText, "Off");
        }

        // Move the text down
        // PosX = -255;
        PosY = -120;
        // Scale = 0.75;

        // Display current modes
        sprintf(tempDisplayBuffer,
                "%s%s\n%s%s\n%s%s",
                "Randomize Coins: ",
                ItemRandoText,
                "Loading Zone Randomizer: ",
                LZRandoText,
                "Loading Zone Randomizer - 1 Hour Challenge: ",
                LZRandoChallengeText);

        drawText(tempDisplayBuffer, PosX, PosY, alpha, color, Scale);

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
        drawMenuWindow();

        // Display the current page
        uint8_t alpha = 0xFF;
        uint32_t TextColor = 0xFFFFFFFF;
        int32_t PosX = 185;
        int32_t PosY = 180;
        float Scale = 0.6;
        char *tempDisplayBuffer = DisplayBuffer; // Prevent DisplayBuffer from being loaded in multiple times

        sprintf(tempDisplayBuffer, "%d/%d", HelpMenuCounter, HelpMenuArraySize);

        drawText(tempDisplayBuffer, PosX, PosY, alpha, TextColor, Scale);

        // Draw the main text
        PosX = -232;
        PosY = 150;
        // Scale = 0.6;

        // Get and draw the text for the current page
        const char *PageText = HelpMenuArray[HelpMenuCounter - 1];
        drawText(PageText, PosX, PosY, alpha, TextColor, Scale);

        // Display page titles
        const uint32_t ItemRandoStartingPage = 2;
        const uint32_t ItemRandoTotalPages = 9;
        const uint32_t LZRandoStartingPage = ItemRandoStartingPage + ItemRandoTotalPages;
        const uint32_t LZRandoTotalPages = 11;
        const uint32_t ChallengeModeStartingPage = LZRandoStartingPage + LZRandoTotalPages;
        const uint32_t ChallengeModeTotalPages = 10;

        // Set up array to use for displaying icons
        int32_t IconPosition[3];
        int32_t *tempIconPosition;
        const int32_t IconPositionX = 0;
        const int32_t IconPositionY = 1;
        IconPosition[2] = 0;

        if ((HelpMenuCounter >= ItemRandoStartingPage) &&
            (HelpMenuCounter <= (ItemRandoStartingPage + ItemRandoTotalPages - 1)))
        {
            switch (HelpMenuCounter)
            {
                case ItemRandoStartingPage + 5:
                {
                    // Check if currently on page 6 of the Item Randomizer section
                    // Draw the icons for each item/badge
                    // Koopa Curse
                    IconPosition[IconPositionX] = -205;
                    IconPosition[IconPositionY] = 93;
                    float scale = 0.6;
                    tempIconPosition = drawIconFromItem(IconPosition, KoopaCurse, scale);

                    // Cake
                    tempIconPosition[IconPositionX] = -204;
                    tempIconPosition[IconPositionY] = 33;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, Cake, scale);

                    // Trade Off
                    tempIconPosition[IconPositionX] = -202;
                    tempIconPosition[IconPositionY] = -10;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, TradeOff, scale);

                    // Debug Badge
                    tempIconPosition[IconPositionX] = -201;
                    tempIconPosition[IconPositionY] = -70;
                    scale = 0.5;
                    tempIconPosition = drawIconFromItem(tempIconPosition, DebugBadge, scale);

                    // Mega Jump
                    tempIconPosition[IconPositionX] = -202;
                    tempIconPosition[IconPositionY] = -128;
                    scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, UpgradedPowerJump, scale);

                    break;
                }
                case ItemRandoStartingPage + 6:
                {
                    // Check if currently on page 7 of the Item Randomizer section
                    // Draw the icons for each item/badge
                    // Mega Smash
                    IconPosition[IconPositionX] = -202;
                    IconPosition[IconPositionY] = 133;
                    float scale = 0.6;
                    tempIconPosition = drawIconFromItem(IconPosition, UpgradedPowerSmash, scale);

                    // Mega Quake
                    // tempIconPosition[IconPositionX] = -202;
                    tempIconPosition[IconPositionY] = 53;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, UpgradedQuakeHammer, scale);

                    // Triple Dip
                    // tempIconPosition[IconPositionX] = -202;
                    tempIconPosition[IconPositionY] = -27;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, TripleDip, scale);

                    // All or Nothing P
                    // tempIconPosition[IconPositionX] = -202;
                    tempIconPosition[IconPositionY] = -87;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, AllorNothingP, scale);

                    // FP Drain P
                    tempIconPosition[IconPositionX] = -196;
                    tempIconPosition[IconPositionY] = -127;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, FPDrainP, scale);

                    // Lucky Day P
                    tempIconPosition[IconPositionX] = -198;
                    tempIconPosition[IconPositionY] = -167;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, LuckyDayP, scale);

                    break;
                }
                case ItemRandoStartingPage + 7:
                {
                    // Check if currently on page 8 of the Item Randomizer section
                    // Draw the icons for each item/badge
                    // Lucky Start P
                    IconPosition[IconPositionX] = -195;
                    IconPosition[IconPositionY] = 133;
                    float scale = 0.6;
                    tempIconPosition = drawIconFromItem(IconPosition, LuckyStartPLuckyStartIcon, scale);

                    // Pity Flower P
                    // tempIconPosition[IconPositionX] = -195;
                    tempIconPosition[IconPositionY] = 53;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, PityFlowerP, scale);

                    // Super Charge
                    // tempIconPosition[IconPositionX] = -195;
                    tempIconPosition[IconPositionY] = 13;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, SuperCharge, scale);

                    // Super Charge P
                    // tempIconPosition[IconPositionX] = -195;
                    tempIconPosition[IconPositionY] = -47;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, SuperChargeP, scale);

                    // Art Attack Badge
                    // tempIconPosition[IconPositionX] = -195;
                    tempIconPosition[IconPositionY] = -107;
                    // scale = 0.6;
                    tempIconPosition = drawIconFromItem(tempIconPosition, InvalidItemBadgeNoKnownEffect, scale);

                    break;
                }
                case ItemRandoStartingPage + 8:
                {
                    // Check if currently on page 9 of the Item Randomizer section
                    // Draw the icons for each item/badge
                    // Run Meter Badge
                    IconPosition[IconPositionX] = -196;
                    IconPosition[IconPositionY] = 133;
                    float scale = 0.55;
                    tempIconPosition = drawIconFromItem(IconPosition, InvalidItemBadgePNoKnownEffect, scale);

                    break;
                }
                default:
                {
                    break;
                }
            }

            // Draw the Item Randomizer page title
            PosX = -80;
            PosY = 180;
            // Scale = 0.6;

            sprintf(tempDisplayBuffer, "%s (%" PRId32 ")", "Item Randomizer", HelpMenuCounter - ItemRandoStartingPage + 1);
        }
        else if ((HelpMenuCounter >= LZRandoStartingPage) && (HelpMenuCounter <= (LZRandoStartingPage + LZRandoTotalPages - 1)))
        {
            switch (HelpMenuCounter)
            {
                case LZRandoStartingPage + 4:
                {
                    // Check if currently on page 5 of the Loading Zone Randomizer section
                    PosX = 0;
                    PosY = 110;
                    // Scale = 0.6;

                    // Draw second column of Loading Zone Randomizer page 5
                    const char *BossesText =
                        "15. Magnus 2\n16. Dark Bones (las_05)\n17. Gloomtail\n18. Shadow Sirens (Ch8)\n19. Grodus\n20. Bowser "
                        "& Kammy\n21. Shadow Queen\n22. Bonetail";
                    drawText(BossesText, PosX, PosY, alpha, TextColor, Scale);

                    break;
                }
#ifndef TTYD_JP
                case LZRandoStartingPage + 6:
                {
                    // Check if currently on page 7 of the Loading Zone Randomizer section
                    // Draw each icon for Power Rush and Power Rush P
                    // Only draw for the US and EU versions, as the JP version does not have the other language icons on the
                    // disc Power Rush
                    tempIconPosition = IconPosition;
                    tempIconPosition[IconPositionX] = 48;
                    tempIconPosition[IconPositionY] = -88;
                    float scale = 0.5;

                    for (uint16_t i = 0; i < 5; i++)
                    {
                        tempIconPosition = drawIcon(tempIconPosition, 73 + i, scale);
                        tempIconPosition[IconPositionX] += 18;
                    }

                    // Power Rush P
                    for (uint16_t i = 0; i < 5; i++)
                    {
                        tempIconPosition = drawIcon(tempIconPosition, 103 + i, scale);
                        tempIconPosition[IconPositionX] += 18;
                    }

                    break;
                }
#endif
                default:
                {
                    break;
                }
            }

            // Draw the Loading Zone Randomizer page title
            PosX = -130;
            PosY = 180;
            // Scale = 0.6;

            sprintf(tempDisplayBuffer,
                    "%s (%" PRId32 ")",
                    "Loading Zone Randomizer",
                    HelpMenuCounter - LZRandoStartingPage + 1);
        }
        else if ((HelpMenuCounter >= ChallengeModeStartingPage) &&
                 (HelpMenuCounter <= (ChallengeModeStartingPage + ChallengeModeTotalPages - 1)))
        {
            // Draw the 1 Hour Challenge page title
            PosX = -80;
            PosY = 180;
            // Scale = 0.6;

            sprintf(tempDisplayBuffer, "%s (%" PRId32 ")", "1 Hour Challenge", HelpMenuCounter - ChallengeModeStartingPage + 1);
        }
        else
        {
            // Exit the function if there is no more text to draw
            return;
        }

        drawText(tempDisplayBuffer, PosX, PosY, alpha, TextColor, Scale);
    }

    void Mod::displayStuff()
    {
        ttyd::dispdrv::dispEntry(
            ttyd::dispdrv::DisplayLayer::kDebug3d,
            0,
            [](ttyd::dispdrv::DisplayLayer layerId, void *user)
            {
                (void)layerId;

                if (LZRando)
                {
                    reinterpret_cast<Mod *>(user)->LZRandoDisplayStuff();

                    if (LZRandoChallenge)
                    {
                        reinterpret_cast<Mod *>(user)->LZRandoChallengeStuff();
                    }
                }

                reinterpret_cast<Mod *>(user)->helpMenu();
            },
            this);

        ttyd::dispdrv::dispEntry(
            ttyd::dispdrv::DisplayLayer::k2d,
            0,
            [](ttyd::dispdrv::DisplayLayer layerId, void *user)
            {
                (void)layerId;

                reinterpret_cast<Mod *>(user)->titleScreenStuff();
                reinterpret_cast<Mod *>(user)->gameModes();
            },
            this);
    }

} // namespace mod