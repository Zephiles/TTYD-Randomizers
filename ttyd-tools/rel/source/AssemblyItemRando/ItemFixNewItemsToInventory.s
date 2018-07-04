.global StartItemFixNewItemsToInventory
.global BranchBackItemFixNewItemsToInventory

StartItemFixNewItemsToInventory:
lis %r30,RandomizeGivenItem@ha
lbz %r29,RandomizeGivenItem@l(%r30)
cmpwi %r29,0 # False
beq+ Exit

# This needs to run to prevent swSet from running
li %r29,0 # False
stb %r29,RandomizeGivenItem@l(%r30)
cmpwi %r29,1 # Arbitrary comparison to set the LT bit
b SkipOriginalComparison

Exit:
# Restore overwritten assembly
cmpwi %r31,240 # PowerJump

SkipOriginalComparison:

BranchBackItemFixNewItemsToInventory:
b 0