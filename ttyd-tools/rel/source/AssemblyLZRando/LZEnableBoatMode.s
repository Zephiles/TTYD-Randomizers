.global StartEnableBoatMode
.global BranchBackEnableBoatMode

#ifdef TTYD_US
  nextmap_offset = 0x16A8
  tik_20_address_low = 0x0358
#elif defined TTYD_JP
  nextmap_offset = 0x1148
  tik_20_address_low = 0x20D8
#elif defined TTYD_EU
  nextmap_offset = 0x1788
  tik_20_address_low = 0xBED8
#endif

StartEnableBoatMode:
# Check if the Loading Zone Randomizer is in use
lis %r4,LZRando@ha
lbz %r4,LZRando@l(%r4)
cmpwi %r4,1 # True
bne+ Exit

# Check if the player already has this upgrade
cmpwi %r3,0 # False
bne+ Exit

# Check if currently in the Rogueport Sewers room with the 3 Shine Sprites
addi %r3,%r13,nextmap_offset
lis %r4,0x802C
ori %r4,%r4,tik_20_address_low
bl strcmp

cmpwi %r3,0 # Strings are the same
li %r3,0 # False, will be used if strings are not the same
bne+ Exit

li %r3,1 # True

Exit:

# Restore overwritten assembly
cmpwi %r3,0

BranchBackEnableBoatMode:
b 0