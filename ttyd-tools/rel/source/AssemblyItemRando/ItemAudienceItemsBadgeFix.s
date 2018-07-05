.global StartAudienceItemsBadgeFix
.global BranchBackAudienceItemsBadgeFix

StartAudienceItemsBadgeFix:
# r3 already contains pouchGetEmptyHaveItemCnt
lwz %r4,0x10(%r24) # Crowd Item
bl audienceFixMarioReceivingBadges

# Restore overwritten assembly
cmpwi %r3,0

BranchBackAudienceItemsBadgeFix:
b 0