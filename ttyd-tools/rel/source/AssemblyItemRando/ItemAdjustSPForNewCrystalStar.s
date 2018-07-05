.global StartAdjustSPForNewCrystalStar
.global BranchBackAdjustSPForNewCrystalStar

StartAdjustSPForNewCrystalStar:
mr %r3,%r31 # Item
bl adjustSPForNewCrystalStar

# Restore overwritten assembly
cmplwi %r30,0 # Need to restore the value from this comparison
sth %r31,0(%r29)

BranchBackAdjustSPForNewCrystalStar:
b 0