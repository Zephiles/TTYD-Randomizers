.global StartEnablePaperTubeModes

#ifdef TTYD_US
  nextmap_offset = 0x16A8
  jin_05_address_low = 0x090C
#elif defined TTYD_JP
  nextmap_offset = 0x1148
  jin_05_address_low = 0x268C
#elif defined TTYD_EU
  nextmap_offset = 0x1788
  jin_05_address_low = 0xC48C
#endif

StartEnablePaperTubeModes:
stwu %sp,-0xC(%sp)
stw %r31,0x8(%sp)
mflr %r31

# Check if the Loading Zone Randomizer is in use
lis %r4,LZRando@ha
lbz %r4,LZRando@l(%r4)
cmpwi %r4,1 # True
bne+ Exit

# Check if the player already has this upgrade
cmpwi %r3,0 # False
bne+ Exit

# Check if currently in the Creepy Steeple room with the Cook Book
addi %r3,%r13,nextmap_offset
lis %r4,0x802C
ori %r4,%r4,jin_05_address_low
bl strcmp

cmpwi %r3,0 # Strings are the same
li %r3,0 # False, will be used if strings are not the same
bne+ Exit

li %r3,1 # True

Exit:
mtlr %r31
lwz %r31,0x8(%sp)
addi %sp,%sp,0xC

# Restore overwritten assembly
cmpwi %r3,0

# Branch back to calling function
blr