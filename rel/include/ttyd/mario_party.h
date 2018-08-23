#pragma once

#include <ttyd/party.h>

namespace ttyd::mario_party {

extern "C" {

// partyGetTechLv
// partyGetHp
// partyChkJoin
void partyLeft(party::PartyMembers id);
void partyJoin(party::PartyMembers id);
uint32_t marioGetExtraPartyId();
uint32_t marioGetPartyId();
// marioGetParty
// marioPartyKill
int32_t marioPartyGoodbye();
int32_t marioPartyHello(party::PartyMembers id);
int32_t marioPartyEntry(party::PartyMembers id);
// unk_JP_US_PAL_062
// marioUseParty

}

}