.global StartCrystalStarPointerWrite
.global BranchBackCrystalStarPointerWrite

StartCrystalStarPointerWrite:
lis %r21,CrystalStarIsInField@ha
lbz %r22,CrystalStarIsInField@l(%r21)
cmpwi %r22,1 # True
bne+ Exit

li %r22,0 # False
stb %r22,CrystalStarIsInField@l(%r21)

lwz %r21,-0x7040(%r13)
lwz %r21,0x4(%r21)
stw %r21,0x1C(%r3) # r3 contains the pointer to where the item was stored

Exit:
# Restore overwritten assembly
lmw %r21,0x14(%sp)

BranchBackCrystalStarPointerWrite:
b 0