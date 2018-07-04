#include <cstdint>

extern "C" {

// --Global Variables--
#ifdef TTYD_US
  uint32_t r13 = 0x8041CF20;
  char *NextBero = reinterpret_cast<char *>(r13 + 0x1688);
  char *NextMap = reinterpret_cast<char *>(r13 + 0x16A8);
  char *NextArea = reinterpret_cast<char *>(r13 + 0x16C8);
  uint32_t seqMainAddress = r13 + 0x1860;
#elif defined TTYD_JP
  uint32_t r13 = 0x80417260;
  char *NextBero = reinterpret_cast<char *>(r13 + 0x1128);
  char *NextMap = reinterpret_cast<char *>(r13 + 0x1148);
  char *NextArea = reinterpret_cast<char *>(r13 + 0x1168);
  uint32_t seqMainAddress = r13 + 0x1300;
#elif defined TTYD_EU
  uint32_t r13 = 0x80429760;
  char *NextBero = reinterpret_cast<char *>(r13 + 0x1768);
  char *NextMap = reinterpret_cast<char *>(r13 + 0x1788);
  char *NextArea = reinterpret_cast<char *>(r13 + 0x17A8);
  uint32_t seqMainAddress = r13 + 0x1940;
#endif

uint32_t GSWAddressesStart = r13 - 0x6F50;

// Display Stuff
bool TimerActive = false;
uint32_t TimerCount = 0;
bool DenyInput = false;

// Item Rando
bool ItemRandoV2 = false;
bool HitShineSprite = false;
bool CrystalStarIsInField = false;
bool EmeraldStarAlreadyChanged = false;
uint16_t CrystalStarNewItem = 1; // Have the Strange Sack used by default as a failsafe
bool MarioRunAwayCoinDrop = false;
bool EnemyHeldItemArrayInUse = false;
uint32_t EnemyHeldItemArray[8] = { 0 };
bool RandomizeGivenItem = false;

// LZ Rando
bool LZRando = false;
bool LZRandoChallenge = false;
bool ReloadCurrentScreen = false;
bool GameOverFlag = false;
bool NewFile = false;
char *NewBero = new char[32]; // 31 bytes for NextBero, 1 byte for NULL
char *NewMap = new char[9]; // 8 bytes for NextMap, 1 byte for NULL
char *LZRandoText = new char[4];
char *LZRandoChallengeText = new char[4];

// LZ Rando Challenge
uint16_t CreditsCount = 0;
bool InCredits = false;
bool BossDefeated[11] = { false };
uint16_t BossCount = 0;

// Clear Cache
bool ClearCacheFlag = false;

}