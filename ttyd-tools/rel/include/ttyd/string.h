#pragma once

#include <cstdint>

namespace ttyd::string {

extern "C" {

// strstr
// strchr
int32_t strncmp(const char *string1, const char *string2, uint32_t amountOfBytesToCompare);
int32_t strcmp(const char *string1, const char *string2);
char *strcat(char *destination, const char *sourceString);
char *strncpy(char *destination, const char *sourceString, uint32_t amountOfBytesToCopy);
char *strcpy(char *destination, const char *sourceString);
int32_t strlen(const char *string);

}

}