.global StartRandomizeCoinBlocks
.global BranchBackRandomizeCoinBlocks

StartRandomizeCoinBlocks:
# Restore overwritten assembly
lwz %r4,0x1A0(%r31)

cmpwi %r4,121 # Coin
bne+ Exit
li %r4,128 # ThunderBolt - Arbitrary value to make sure it gets randomized

Exit:

BranchBackRandomizeCoinBlocks:
b 0