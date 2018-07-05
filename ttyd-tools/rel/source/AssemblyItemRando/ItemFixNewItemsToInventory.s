.global StartItemFixNewItemsToInventory
.global BranchBackItemFixNewItemsToInventory

StartItemFixNewItemsToInventory:
mr %r3,%r31 # Item
bl newItemsPrevent_swSet

# Restore overwritten assembly with modification
cmpwi %r3,240 # PowerJump

BranchBackItemFixNewItemsToInventory:
b 0