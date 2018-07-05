#pragma once

namespace ttyd::mario_party {

extern "C" {

// partyGetTechLv
// partyGetHp
// partyChkJoin
// partyLeft
void partyJoin(uint32_t id);
uint32_t marioGetExtraPartyId();
uint32_t marioGetPartyId();
// marioGetParty
// marioPartyKill
// marioPartyGoodbye
int32_t marioPartyHello(uint32_t id);
// marioPartyEntry
// unk_JP_US_PAL_062
// marioUseParty

}

}