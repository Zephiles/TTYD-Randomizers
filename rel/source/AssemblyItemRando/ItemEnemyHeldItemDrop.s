.global StartWriteEnemyHeldItemDrop
.global BranchBackWriteEnemyHeldItemDrop

StartWriteEnemyHeldItemDrop:
stwu %sp,-0xC(%sp)
stw %r31,0x8(%sp)
mr %r31,%r4

bl getEnemyItemDrop
mr %r5,%r3

mr %r4,%r31
lwz %r31,0x8(%sp)
addi %sp,%sp,0xC

# Restore overwritten assembly
mr %r3,%r30

BranchBackWriteEnemyHeldItemDrop:
b 0