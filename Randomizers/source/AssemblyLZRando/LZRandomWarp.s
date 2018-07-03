.global StartGetRandomWarp
.global BranchBackGetRandomWarp

StartGetRandomWarp:
# Check if the Loading Zone Randomizer is in use
lis %r3,LZRando@ha
lbz %r3,LZRando@l(%r3)
cmpwi %r3,1 # True
bne+ Exit

bl getRandomWarp

Exit:
# Restore overwritten assembly
li %r3,512

BranchBackGetRandomWarp:
b 0