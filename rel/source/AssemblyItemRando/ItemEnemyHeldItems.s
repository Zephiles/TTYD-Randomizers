.global StartWriteEnemyHeldItems
.global BranchBackWriteEnemyHeldItems

StartWriteEnemyHeldItems:
stwu %sp,-0x18(%sp)
stmw %r29,0x8(%sp)
mr %r29,%r3
mr %r30,%r4
mr %r31,%r5

mr %r3,%r27 # OriginalEnemyHeldItemArray
mr %r4,%r5 # ArrayIndex
bl assignEnemyHeldItem
mr %r0,%r3

mr %r3,%r29
mr %r4,%r30
mr %r5,%r31
lmw %r29,0x8(%sp)
addi %sp,%sp,0x18

BranchBackWriteEnemyHeldItems:
b 0