.global StartWriteEnemyHeldItems
.global BranchBackWriteEnemyHeldItems

StartWriteEnemyHeldItems:

stwu %sp,-0x18(%sp)
stmw %r29,0x8(%sp)
mr %r29,%r3
mr %r30,%r4
mr %r31,%r5

# Set EnemyHeldItemArrayInUse to true
li %r4,1 # true
lis %r3,EnemyHeldItemArrayInUse@ha
stb %r4,EnemyHeldItemArrayInUse@l(%r3)

GetRandomItem:
bl randomizeItem
cmpwi %r3,126 #GoldBar
blt- GetRandomItem

mr %r0,%r3
mr %r5,%r31 # Restore original value
lis %r4,EnemyHeldItemArray@h
ori %r4,%r4,EnemyHeldItemArray@l

addi %r3,%r5,-0x1C # Index
stwx %r0,%r4,%r3 # Add to EnemyHeldItemArray
stwx %r0,%r27,%r5 # Store in regular place

mr %r3,%r29
mr %r4,%r30
lmw %r29,0x8(%sp)
addi %sp,%sp,0x18

BranchBackWriteEnemyHeldItems:
b 0