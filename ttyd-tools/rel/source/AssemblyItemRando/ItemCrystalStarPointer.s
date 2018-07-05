.global StartCrystalStarPointerWrite
.global BranchBackCrystalStarPointerWrite

StartCrystalStarPointerWrite:
# r3 already contains CurrentItemLocation
bl writeCrystalStarPointer

# Restore overwritten assembly
lmw %r21,0x14(%sp)

BranchBackCrystalStarPointerWrite:
b 0