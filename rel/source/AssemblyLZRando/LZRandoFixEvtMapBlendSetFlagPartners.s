.global StartFixEvtMapBlendSetFlagPartners
.global BranchBackFixEvtMapBlendSetFlagPartners

StartFixEvtMapBlendSetFlagPartners:
# r3 already contains PartnerPointer
bl fixEvtMapBlendSetFlagPartners

# Restore overwritten instruction
cmpwi %r31,1

BranchBackFixEvtMapBlendSetFlagPartners:
b 0