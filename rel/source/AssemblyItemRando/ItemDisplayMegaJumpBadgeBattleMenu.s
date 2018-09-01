.global StartDisplayMegaJumpBadgeBattleMenu
.global BranchBackDisplayMegaJumpBadgeBattleMenu

StartDisplayMegaJumpBadgeBattleMenu:
stwu %sp,-0x14(%sp)
stmw %r30,0x8(%sp)
mr %r31,%r4
mr %r30,%r5

mr %r3,%r0 # CheckBit
bl displayMegaJumpBadgeInMenu

mr %r4,%r31
mr %r5,%r30
lmw %r30,0x8(%sp)
addi %sp,%sp,0x14

# Check the returned bool
cmpwi %r3,0 # False

BranchBackDisplayMegaJumpBadgeBattleMenu:
b 0