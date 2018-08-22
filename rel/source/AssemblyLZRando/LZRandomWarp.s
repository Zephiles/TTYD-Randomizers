.global StartGetRandomWarp
.global BranchBackGetRandomWarp

StartGetRandomWarp:
bl getRandomWarp

# Restore overwritten assembly
li %r3,512

BranchBackGetRandomWarp:
b 0