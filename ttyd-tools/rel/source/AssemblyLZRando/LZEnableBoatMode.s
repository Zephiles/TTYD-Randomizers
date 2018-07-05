.global StartEnableBoatMode
.global BranchBackEnableBoatMode

StartEnableBoatMode:
# r3 already contains pouchCheckItem
bl enableBoatMode

# Restore overwritten assembly
cmpwi %r3,0

BranchBackEnableBoatMode:
b 0