#pragma once

#include <cstdint>

namespace managestrings {

char *strncpy_String(char *dest, const char *src, uint32_t size);
char *strcpy_String(char *dest, const char *src);
char *strcpy_NextBero(const char *string);
char *strcpy_NextMap(const char *string);
bool strncmp_String(char *str1, const char *str2, uint32_t size);
bool strcmp_String(char *str1, const char *str2);
bool strcmp_NextBero(const char *string);
bool strcmp_NextMap(const char *string);

}