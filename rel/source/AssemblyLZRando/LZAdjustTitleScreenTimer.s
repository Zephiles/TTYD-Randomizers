.global StartAdjustTitleScreenTimer
.global BranchBackAdjustTitleScreenTimer

# The function needs to be different for JP
#ifndef TTYD_JP
  StartAdjustTitleScreenTimer:
  stwu %sp,-0x10(%sp)
  stmw %r30,0x8(%sp)
  mr %r31,%r3
  mr %r30,%r4

  # r3 already contains currentTime
  bl adjustTitleScreenTimer
  mr %r0,%r3

  mr %r4,%r30
  mr %r3,%r31
  lmw %r30,0x8(%sp)
  addi %sp,%sp,0x10
#else
  StartAdjustTitleScreenTimer:
  mr %r3,%r0 # currentTime
  # r4 already contains titleWorkPointer2
  bl adjustTitleScreenTimer

  # Check the returned value
  cmpwi %r3,0 # Check if less than or equal to 0
#endif

BranchBackAdjustTitleScreenTimer:
b 0