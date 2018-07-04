.global StartWriteEnemyHeldItemArrayInUseFlag
.global BranchBackWriteEnemyHeldItemArrayInUseFlag

#ifdef TTYD_US
  offset = 0x2D8
#elif defined TTYD_JP
  offset = 0x2C0
#elif defined TTYD_EU
  offset = 0x2D8
#endif

StartWriteEnemyHeldItemArrayInUseFlag:
# Restore overwritten assembly
sth %r3,offset(%r27)

li %r26,0 # False
lis %r28,EnemyHeldItemArrayInUse@ha
stb %r26,EnemyHeldItemArrayInUse@l(%r28)

BranchBackWriteEnemyHeldItemArrayInUseFlag:
b 0