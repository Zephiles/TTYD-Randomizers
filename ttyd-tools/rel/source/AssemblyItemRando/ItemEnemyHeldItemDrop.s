.global StartWriteEnemyHeldItemDrop
.global BranchBackWriteEnemyHeldItemDrop

StartWriteEnemyHeldItemDrop:

stwu %sp,-0x1C(%sp)
stmw %r28,0x8(%sp)
mr %r31,%r4

lis %r30,EnemyHeldItemArray@h
ori %r30,%r30,EnemyHeldItemArray@l
subi %r29,%r30,0x4
li %r28,8 # Size of EnemyHeldItemArray
li %r3,0 # Counter

CheckEnemyItems:
lwzu %r0,0x4(%r29)
cmpwi %r0,0
beq+ Loop
addi %r3,%r3,1

Loop:
subic. %r28,%r28,1
bne+ CheckEnemyItems

# Get a random value for the index
bl irand
rlwinm %r3,%r3,2,0,29 # Multiply by 0x4
lwzx %r5,%r30,%r3 # Item to drop

mr %r4,%r31
lmw %r28,0x8(%sp)
addi %sp,%sp,0x1C

# Restore overwritten assembly
mr %r3,%r30

BranchBackWriteEnemyHeldItemDrop:
b 0