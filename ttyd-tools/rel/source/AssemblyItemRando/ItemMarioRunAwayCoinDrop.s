.global StartMarioRunAwayCoinDrop
.global BranchBackMarioRunAwayCoinDrop

StartMarioRunAwayCoinDrop:
li %r7,1 # True
lis %r6,MarioRunAwayCoinDrop@ha
stb %r7,MarioRunAwayCoinDrop@l(%r6)

# Restore overwritten assembly
li %r6,-1

BranchBackMarioRunAwayCoinDrop:
b 0