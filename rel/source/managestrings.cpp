#include "managestrings.h"

#include <ttyd/string.h>

#include <cstdint>

extern char *NextBero;
extern char *NextMap;

namespace managestrings {

char *strncpy_String(char *dest, const char *src, uint32_t size)
{
  return ttyd::string::strncpy(dest, src, size);
}

char *strcpy_String(char *dest, const char *src)
{
  return ttyd::string::strcpy(dest, src);
}

char *strcpy_NextBero(const char *string)
{
  return strcpy_String(NextBero, string);
}

char *strcpy_NextMap(const char *string)
{
  return strcpy_String(NextMap, string);
}

bool strncmp_String(char *str1, const char *str2, uint32_t size)
{
  return ttyd::string::strncmp(str1, str2, size) == 0;
}

bool strcmp_String(char *str1, const char *str2)
{
  return ttyd::string::strcmp(str1, str2) == 0;
}

bool strcmp_NextBero(const char *string)
{
  return strcmp_String(NextBero, string);
}

bool strcmp_NextMap(const char *string)
{
  return strcmp_String(NextMap, string);
}

}