#include "drawstring.h"

#include <ttyd/string.h>
#include <ttyd/__mem.h>
#include <ttyd/fontmgr.h>

#include <cstdint>

namespace drawstring {

void drawStringMultiline(float x, float y, const char *text)
{
  int32_t StringSize = ttyd::string::strlen(text);
  char *line = new char[StringSize + 1];
  uint32_t counter = 0;
  uint32_t currentIndex = 0;
  bool startOfNewLine = true;
  bool writeLine = false;
  
  for (int i = 0; i < StringSize; i++)
  {
    if (text[i] != '\n')
    {
      counter++;
      
      if (startOfNewLine)
      {
        startOfNewLine = false;
        currentIndex = i;
      }
    }
    else
    {
      writeLine = true;
    }
    
    if (writeLine || ((i + 1) == StringSize))
    {
      ttyd::__mem::memset(line, 0, StringSize);
      ttyd::string::strncpy(line, &text[currentIndex], counter);
      ttyd::fontmgr::FontDrawString(x, y, line);
      
      writeLine = false;
      startOfNewLine = true;
      counter = 0;
      y -= 20;
    }
  }
  
  delete[] (line);
}

}