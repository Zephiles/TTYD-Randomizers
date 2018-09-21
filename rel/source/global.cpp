#include <cstdint>

extern "C" {

// --Global Variables--
#ifdef TTYD_US
  uint32_t r13 = 0x8041CF20;
  char *NextBero = reinterpret_cast<char *>(r13 + 0x1688);
  char *NextMap = reinterpret_cast<char *>(r13 + 0x16A8);
  char *NextArea = reinterpret_cast<char *>(r13 + 0x16C8);
  uint32_t seqMainAddress = r13 + 0x1860;
  uint32_t BattleAddressesStart = r13 + 0x1C70;
  // uint32_t AreaObjectsAddressesStart = 0x803D98A8;
  // uint32_t NPCAddressesStart = r13 + 0x19A0;
  uint32_t _mapEntAddress = r13 + 0x1740;
  uint32_t wp_fadedrv_Address = r13 + 0x1700;
#elif defined TTYD_JP
  uint32_t r13 = 0x80417260;
  char *NextBero = reinterpret_cast<char *>(r13 + 0x1128);
  char *NextMap = reinterpret_cast<char *>(r13 + 0x1148);
  char *NextArea = reinterpret_cast<char *>(r13 + 0x1168);
  uint32_t seqMainAddress = r13 + 0x1300;
  uint32_t BattleAddressesStart  = r13 + 0x1710;
  // uint32_t AreaObjectsAddressesStart = 0x803CA2D8;
  // uint32_t NPCAddressesStart = r13 + 0x1448;
  uint32_t _mapEntAddress = r13 + 0x11E0;
  uint32_t wp_fadedrv_Address = r13 + 0x11A0;
#elif defined TTYD_EU
  uint32_t r13 = 0x80429760;
  char *NextBero = reinterpret_cast<char *>(r13 + 0x1768);
  char *NextMap = reinterpret_cast<char *>(r13 + 0x1788);
  char *NextArea = reinterpret_cast<char *>(r13 + 0x17A8);
  uint32_t seqMainAddress = r13 + 0x1940;
  uint32_t BattleAddressesStart  = r13 + 0x1D50;
  // uint32_t AreaObjectsAddressesStart = 0x803DA6F8;
  // uint32_t NPCAddressesStart = r13 + 0x1A80;
  uint32_t _mapEntAddress = r13 + 0x1820;
  uint32_t wp_fadedrv_Address = r13 + 0x17E0;
#endif

uint32_t GSWAddressesStart = r13 - 0x6F50;
uint32_t CrystalStarPointerAddress = r13 - 0x7040;
// uint32_t AreaLZsAddressesStart = r13 - 0x6F58;
uint32_t imgEntryAddress = r13 - 0x6F18;

// Display Stuff
char *DisplayBuffer = new char[256];
bool ShowScoreSources = false;
bool TimerDisabled = false;
bool TimerActive = false;
uint32_t TimerCount = 0;
bool DisablePlayerControl = false;
uint8_t HelpMenuCounter = 0;
uint8_t PreviousHelpMenuCounterValue = 0;

// Item Rando
bool RandomizeCoins = false;
bool HitShineSprite = false;
bool CrystalStarIsInField = false;
bool EmeraldStarAlreadyChanged = false;
uint16_t CrystalStarNewItem = 1; // Have the Strange Sack used by default as a failsafe
bool MarioRunAwayCoinDrop = false;
bool EnemyHeldItemArrayInUse = false;
int8_t EnemyHeldItemArrayCounter = 0;
bool RanAwayFromBattle = false;
uint16_t EnemyHeldItemArray[8][8] = { 0 };
bool RandomizeGivenItem = false;
char *ItemRandoText = new char[4]; // 3 bytes for ItemRandoText, 1 byte for NULL

// LZ Rando
bool LZRando = false;
bool LZRandoChallenge = false;
bool ReloadCurrentScreen = false;
bool GameOverFlag = false;
bool NewFile = false;
bool MarioFreeze = false;
char *NewBero = new char[32]; // 31 bytes for NextBero, 1 byte for NULL
char *NewMap = new char[9]; // 8 bytes for NextMap, 1 byte for NULL
bool ClearCacheFlag = false;
bool SQWarpAway = false;
char *LZRandoText = new char[4]; // 3 bytes for LZRandoText, 1 byte for NULL

// LZ Rando Challenge
uint8_t TimesUpCounter = 0;
uint8_t JustDefeatedBoss = 0;
bool BossDefeated[19] = { false };
uint32_t BossScore = 0;
bool InGameOver = false;
uint16_t GameOverCount = 0;
int32_t FinalScore = 0;
int32_t DisplayScores[10] = { 0 };
char *LZRandoChallengeText = new char[4]; // 3 bytes for LZRandoChallengeText, 1 byte for NULL

}