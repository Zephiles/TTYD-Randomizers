.global StartEnableWalkJumpOnWater

StartEnableWalkJumpOnWater:
stwu %sp,-0xC(%sp)
stw %r31,0x8(%sp)
mflr %r31

# r3 already contains CheckBit
bl enableWalkOnWater

mtlr %r31
lwz %r31,0x8(%sp)
addi %sp,%sp,0xC

# Check the returned bool
cmpwi %r3,0 # False

# Branch back to calling function
blr