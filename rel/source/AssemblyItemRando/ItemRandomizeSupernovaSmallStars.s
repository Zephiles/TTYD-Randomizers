.global StartRandomizeSupernovaSmallStars

StartRandomizeSupernovaSmallStars:
stwu %sp,-0x1C(%sp)
stmw %r28,0x8(%sp)
mr %r29,%r3
mr %r30,%r4
mr %r31,%r6
mflr %r28

bl randomizeSupernovaSmallStars
mr %r5,%r3

mtlr %r28
mr %r3,%r29
mr %r4,%r30
mr %r6,%r31
lmw %r28,0x8(%sp)
addi %sp,%sp,0x1C

# Branch back to calling function
blr