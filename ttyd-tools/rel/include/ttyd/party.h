#pragma once

namespace ttyd::party {

extern "C" {

// partySetFamicomMode
// partyShadowOff
// partyUpdateKeyData
// partyClearCont
// partyKeyOff
// partyKeyOn
// partyDisp
// partyPreDisp
// partyMakeDispDir
// partySetCamId
// partyInitCamId
// party_damage_return
// partyRideYoshiMain
// party_ride_yoshi_force_move
// party_force_ride_yoshi
// partyRideMain
// partyRideChk
// allPartyRideOff2
// allPartyRideOff
// unk_JP_US_PAL_029
// allPartyForceRideOn
// getRidePos
// allPartyRideShip
// allPartyRidePlane
// partyDokanEnd
// partyDokanMain
// partyDokanInit
// partyGoodbyeMain
// partyGoodbyeInit
// partyHelloMain
// partyHello
// partyDoWork
// driveParty
// partyMain
// unk_JP_US_PAL_030
// unk_JP_US_PAL_031
// unk_JP_US_PAL_032
// partyChgRunMode
// partyUsePost
// partyReInit
// partyInit
// partyEntry2Pos
// partyEntry2Hello
// partyEntry2
// partyEntryMain
// partyGetHeight
// yoshiSetColor
// partyKill2
// partyKill
// partyGoodbye
// partyEntryPos
// partyEntry
// partyEntryHello
// partySetForceMove
// partyStop
// partyRun
// partyCtrlOn
// partyCtrlOff
// partyChgPaper
// partyChgPose
// partyChgPoseId
// partyPaperLightOff
// partyPaperOff
// partyPaperOn
// anotherPartyGetPtr
void *partyGetPtr(uint32_t partyId);

}

}