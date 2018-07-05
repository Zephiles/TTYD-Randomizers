.global StartNewItemsToInventory
.global BranchBackNewItemsToInventory

StartNewItemsToInventory:
stwu %sp,-0xC(%sp)
stw %r30,0x8(%sp)
mr %r30,%r3

lwz %r3,0(%r31) # Item/Expression
bl newItemToInventory
mr %r4,%r3

mr %r3,%r30
lwz %r30,0x8(%sp)
addi %sp,%sp,0xC

BranchBackNewItemsToInventory:
b 0