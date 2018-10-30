#include "mod.h"

#include <ttyd/string.h>

#include <cstdio>

extern char *NextMap;
extern char *NextArea;

namespace mod {

void displayBacktraceScreen()
{
  #ifdef TTYD_US
    uint32_t DebugModeInitializeAddress = 0x80009B2C;
  #elif defined TTYD_JP
    uint32_t DebugModeInitializeAddress = 0x8000999C;
  #elif defined TTYD_EU
    uint32_t DebugModeInitializeAddress = 0x80009CF0;
  #endif
  
  // Display the backtrace screen by default
  *reinterpret_cast<uint32_t *>(DebugModeInitializeAddress) = 0x3800FFFF; // li r0,-1
}

// Only modify with in the US and EU versions of the game
#ifndef TTYD_JP
void edit_backtrace_screen()
{
  #ifdef TTYD_US
    uint32_t FontSizeAddress = 0x80428BC0;
    uint32_t PPCHaltBranchAddress = 0x8025E4A4;
    uint32_t EndBranchAddress = 0x8025E4A8;
  #elif defined TTYD_EU
    uint32_t FontSizeAddress = 0x804356C8;
    uint32_t PPCHaltBranchAddress = 0x8026207C;
    uint32_t EndBranchAddress = 0x80262080;
  #endif
  
  *reinterpret_cast<float *>(FontSizeAddress) = 0.65;
  *reinterpret_cast<uint32_t *>(PPCHaltBranchAddress) = 0x3B400000; // li r26,0
  *reinterpret_cast<uint32_t *>(EndBranchAddress) = 0x4BFFFDD4; // b -0x22C
}
#endif

void msgWindow_Entry__strlen__allocation__fix()
{
  // Fix a bug with how the game allocates memory in this function after calling strlen
  #ifdef TTYD_US
    uint32_t msgWindowMrAddress = 0x800816F4;
  #elif defined TTYD_JP
    uint32_t msgWindowMrAddress = 0x80080B6C;
  #elif defined TTYD_EU
    uint32_t msgWindowMrAddress = 0x800829b0;
  #endif
  
  *reinterpret_cast<uint32_t *>(msgWindowMrAddress) = 0x38830001; // addi r4,r3,1
}

void adjustTitleScreenTimer()
{
  #ifdef TTYD_US
    // Set the instruction to multiple by 8 instead of 20, so that it checks for 7 seconds total
    uint32_t setTotalTimeAddress = 0x800096B4;
    *reinterpret_cast<uint32_t *>(setTotalTimeAddress) = 0x1C030008; // mulli r0,r3,8
  #elif defined TTYD_JP
    // Set the instruction to compare to 420, which is 7 seconds in frames
    uint32_t setTotalTimeAddress = 0x80009534;
    *reinterpret_cast<uint32_t *>(setTotalTimeAddress) = 0x2C0001A4; // cmpwi r0,420
  #elif defined TTYD_EU
    // Set the instruction to multiple by 8 instead of 20, so that it checks for 7 seconds total
    uint32_t setTotalTimeAddress = 0x80009878;
    *reinterpret_cast<uint32_t *>(setTotalTimeAddress) = 0x1C030008; // mulli r0,r3,8
  #endif
}

void writeNextMap()
{
  // These are normally not set during the boot process, which can cause issues with string comparisons with other code
  ttyd::string::strcpy(NextMap, "dmo_00");
  ttyd::string::strncpy(NextArea, "dmo_00", 3);
}

void Mod::writeOnce()
{
  Mod::writeItemRandoAssemblyPatches();
  Mod::writeLZRandoAssemblyPatches();
  displayBacktraceScreen();
  msgWindow_Entry__strlen__allocation__fix();
  adjustTitleScreenTimer();
  writeNextMap();
  
  // Only run with in the US and EU versions of the game
  #ifndef TTYD_JP
    edit_backtrace_screen();
  #endif
}

}