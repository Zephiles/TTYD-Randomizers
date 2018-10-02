.global StartEnablePlaneMode
.global BranchBackEnablePlaneMode

StartEnablePlaneMode:
# r3 already contains pouchCheckItem
bl enablePlaneMode

# Restore overwritten assembly
cmpwi %r3,0

BranchBackEnablePlaneMode:
b 0