.global StartEnableLandOnWater
.global BranchBackEnableLandOnWater

StartEnableLandOnWater:
stwu %sp,-0xC(%sp)
stw %r31,0x8(%sp)
mr %r31,%r4

# r3 already contains CheckBit
bl enableWalkOnWater

mr %r4,%r31
lwz %r31,0x8(%sp)
addi %sp,%sp,0xC

# Check the returned bool
cmpwi %r3,0 # False

BranchBackEnableLandOnWater:
b 0