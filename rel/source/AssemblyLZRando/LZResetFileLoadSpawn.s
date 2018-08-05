.global StartResetFileLoadSpawn
.global BranchBackResetFileLoadSpawn

StartResetFileLoadSpawn:
# Restore overwritten assembly
stw %r0,0x11B8(%r3)

# r3 already contains GSWAddressesPointer
bl resetFileLoadSpawn

BranchBackResetFileLoadSpawn:
b 0