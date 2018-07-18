#pragma once

namespace ttyd::mario_party {

extern "C" {

// partyGetTechLv
// partyGetHp
// partyChkJoin
void partyLeft(uint32_t id);
void partyJoin(uint32_t id);
uint32_t marioGetExtraPartyId();
uint32_t marioGetPartyId();
// marioGetParty
// marioPartyKill
int32_t marioPartyGoodbye();
int32_t marioPartyHello(uint32_t id);
// marioPartyEntry
// unk_JP_US_PAL_062
// marioUseParty

}

}