.global StartWriteRunAwayFlag
.global BranchBackWriteRunAwayFlag

#ifdef TTYD_US
  offset = 0x2D8
#elif defined TTYD_JP
  offset = 0x2C0
#elif defined TTYD_EU
  offset = 0x2D8
#endif

StartWriteRunAwayFlag:
# Restore overwritten assembly
sth %r3,offset(%r27)

li %r26,1 # True
lis %r28,RanAwayFromBattle@ha
stb %r26,RanAwayFromBattle@l(%r28)

BranchBackWriteRunAwayFlag:
b 0