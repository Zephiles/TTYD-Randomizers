#include "items.h"

#include <cstdint>

extern "C"
{
// --Global Variables--
#ifdef TTYD_US
    uint32_t r13 = 0x8041CF20;
    char *NextBero = reinterpret_cast<char *>(r13 + 0x1688);
    char *NextMap = reinterpret_cast<char *>(r13 + 0x16A8);
    char *NextArea = reinterpret_cast<char *>(r13 + 0x16C8);
    uint32_t seqMainAddress = r13 + 0x1860;
    uint32_t BattleAddressesStart = r13 + 0x1C70;
    // uint32_t AreaObjectsAddressesStart = 0x803D98A8;
    uint32_t NPCAddressesStart = r13 + 0x19A0;
    uint32_t _mapEntAddress = r13 + 0x1740;
    uint32_t wp_fadedrv_Address = r13 + 0x1700;
    uint32_t winMgrAddress = r13 + 0xC20;
#elif defined TTYD_JP
    uint32_t r13 = 0x80417260;
    char *NextBero = reinterpret_cast<char *>(r13 + 0x1128);
    char *NextMap = reinterpret_cast<char *>(r13 + 0x1148);
    char *NextArea = reinterpret_cast<char *>(r13 + 0x1168);
    uint32_t seqMainAddress = r13 + 0x1300;
    uint32_t BattleAddressesStart = r13 + 0x1710;
    // uint32_t AreaObjectsAddressesStart = 0x803CA2D8;
    uint32_t NPCAddressesStart = r13 + 0x1448;
    uint32_t _mapEntAddress = r13 + 0x11E0;
    uint32_t wp_fadedrv_Address = r13 + 0x11A0;
    uint32_t winMgrAddress = r13 + 0x6C0;
#elif defined TTYD_EU
    uint32_t r13 = 0x80429760;
    char *NextBero = reinterpret_cast<char *>(r13 + 0x1768);
    char *NextMap = reinterpret_cast<char *>(r13 + 0x1788);
    char *NextArea = reinterpret_cast<char *>(r13 + 0x17A8);
    uint32_t seqMainAddress = r13 + 0x1940;
    uint32_t BattleAddressesStart = r13 + 0x1D50;
    // uint32_t AreaObjectsAddressesStart = 0x803DA6F8;
    uint32_t NPCAddressesStart = r13 + 0x1A80;
    uint32_t _mapEntAddress = r13 + 0x1820;
    uint32_t wp_fadedrv_Address = r13 + 0x17E0;
    uint32_t winMgrAddress = r13 + 0xD00;
#endif

    uint32_t GSWAddressesStart = r13 - 0x6F50;
    uint32_t CrystalStarPointerAddress = r13 - 0x7040;
    // uint32_t AreaLZsAddressesStart = r13 - 0x6F58;
    uint32_t TitleWorkPointer2 = r13 - 0x7F80;

    // Display Stuff
    bool TransparentTextFlag = false;
    char DisplayBuffer[256] = {0};
    bool ShowScoreSources = false;
    bool TimerDisabled = false;
    bool TimerActive = false;
    uint32_t TimerCount = 0;
    bool DisablePlayerControl = false;
    uint8_t HelpMenuCounter = 0;
    uint8_t PreviousHelpMenuCounterValue = 0;

    // Item Rando
    uint16_t FieldMostRecentItem = ThunderBolt; // Have a Thunder Bolt used by default as a failsafe
    bool HitShineSprite = false;
    bool CrystalStarIsInField = false;
    bool EmeraldStarAlreadyChanged = false;
    uint16_t CrystalStarNewItem = StrangeSack; // Have a Strange Sack used by default as a failsafe
    bool MarioRunAwayCoinDrop = false;
    bool HitMultiCoinBlock = false;
    bool EnemyHeldItemArrayInUse = false;
    bool RandomizeCoins = false;
    int8_t EnemyHeldItemArrayCounter = 0;
    bool RanAwayFromBattle = false;
    uint16_t EnemyHeldItemArray[8][8] = {0};
    bool RandomizeGivenItem = false;

    // LZ Rando
    bool LZRandoChallenge = false;
    bool LZRando = false;
    bool ReloadCurrentScreen = false;
    bool ChangedLZ = false;
    bool GameOverFlag = false;
    bool NewFile = false;
    uint16_t DefeatedRawkHawkCounter = 0;
    bool TransformIntoShip = false;
    bool CloseCurtainFlag = false;
    bool MarioFreeze = false;
    bool ClearCacheFlag = false;
    char NewBero[32] = {0}; // 31 bytes for NextBero, 1 byte for NULL
    char NewMap[9] = {0};   // 8 bytes for NextMap, 1 byte for NULL
    bool CheckCurrentTextboxFlag = false;
    bool ResetSystemFlag = false;

    // LZ Rando Challenge
    int32_t *AllFinalScoresArray = nullptr;
    uint32_t TotalFinalScoresCounter = 0;
    bool FinishedARun = false;
    uint8_t FinishedRunCounter = 0;
    int32_t FinalScore = 0;
    bool GameOverChallengeMode = false;
    uint32_t FinalScoresMenuCounter = 0;
    uint8_t JustDefeatedBoss = 0;
    bool BossDefeated[22] = {false};
    uint32_t BossScore = 0;
    bool InGameOver = false;
    uint16_t GameOverCount = 0;
    uint8_t TimesUpCounter = 0;
}
