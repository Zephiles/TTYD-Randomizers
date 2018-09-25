.global StartMultiCoinBlock

StartMultiCoinBlock:
li %r3,1 # True
lis %r4,HitMultiCoinBlock@ha
stb %r3,HitMultiCoinBlock@l(%r4)

# Restore overwritten assembly
li %r3,0

# Branch back to calling function
blr