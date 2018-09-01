.global StartPreventBlooperCrash2
.global BranchBackPreventBlooperCrash2

#ifdef TTYD_US
  offset = 0x218
#elif defined TTYD_JP
  offset = 0x214
#elif defined TTYD_EU
  offset = 0x218
#endif

StartPreventBlooperCrash2:
stwu %sp,-0x10(%sp)
stmw %r30,0x8(%sp)
mr %r31,%r3
mr %r30,%r4

# Check if the BattleUnitPointer is valid or not
mr %r3,%r4 # BattleUnitPointer
bl checkBattleUnitPointer

# Check the returned bool
cmpwi %r3,0 # False
beq- ExitFunction

mr %r3,%r31
mr %r4,%r30
lmw %r30,0x8(%sp)
addi %sp,%sp,0x10

# Restore overwritten assembly
lwz %r5,offset(%r4)

BranchBackPreventBlooperCrash2:
b 0

ExitFunction:
lmw %r28,0x20(%sp)
li %r3,2
lwz %r0,0x34(%sp)
mtlr %r0
addi %sp,%sp,48
blr