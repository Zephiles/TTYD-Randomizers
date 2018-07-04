.global StartAudienceItemsBadgeFix
.global BranchBackAudienceItemsBadgeFix

StartAudienceItemsBadgeFix:
lwz %r0,0x10(%r24) # Crowd Item
cmpwi %r0,236 # CrowdCan
blt- Exit

bl pouchGetHaveBadgeCnt
cmpwi %r3,200 # Total Badge Slots
bne+ SpaceIsFree

# Return 0 empty spaces
li %r3,0
b Exit

# Return 1 or more empty spaces
SpaceIsFree:
li %r3,1

Exit:
# Restore overwritten assembly
cmpwi %r3,0

BranchBackAudienceItemsBadgeFix:
b 0