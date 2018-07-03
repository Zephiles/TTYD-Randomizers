.global StartAdjustSPForNewCrystalStar
.global BranchBackAdjustSPForNewCrystalStar

StartAdjustSPForNewCrystalStar:
cmpwi %r31,96 #MagicalMapBigger
beq- GetRandomItem
cmpwi %r31,120 #CrystalStar
bgt+ Exit
cmpwi %r31,114 #DiamondStar
blt+ Exit

# Only run if the current item is a crystal star or the magical map
GetRandomItem:
mr %r3,%r31 # Item
bl adjustSPForNewCrystalStar

Exit:
# Restore overwritten assembly
cmplwi %r30,0 # Need to restore the value from this comparison
sth %r31,0(%r29)

BranchBackAdjustSPForNewCrystalStar:
b 0