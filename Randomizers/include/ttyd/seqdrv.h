#pragma once

#include <cstdint>

namespace ttyd::seqdrv {

enum class SeqIndex : int32_t
{
  kLogo = 0,
  kTitle,
  kGame,
  kMapChange,
  kBattle,
  kGameOver,
  kLoad,
  kE3,
};

struct SeqInfo
{
  SeqIndex seq;
  int32_t state;
  const char *mapName;
  const char *beroName;
  uint32_t counter;
  uint32_t unk_14;
  uint32_t unk_18;
  uint32_t unk_1c;
} __attribute__((__packed__));

extern "C" {

void seqInit_MARIOSTORY();
void seqMain();
void seqSetSeq(SeqIndex seq, const char *mapName, const char *beroName);
int32_t seqGetSeq();
int32_t seqGetPrevSeq();
int32_t seqGetNextSeq();
bool seqCheckSeq();

}

}