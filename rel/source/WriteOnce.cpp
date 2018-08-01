#include "mod.h"

#include <ttyd/string.h>

#include <cstdio>

extern char *NextMap;
extern char *NextArea;

namespace mod {

void backtraceScreen()
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
  backtraceScreen();
  writeNextMap();
}

}