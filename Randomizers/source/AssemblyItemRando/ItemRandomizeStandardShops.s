.global StartRandomizeStandardShops
.global BranchBackRandomizeStandardShops

StartRandomizeStandardShops:
stwu %sp,-0xC(%sp)
stw %r31,0x8(%sp)
mr %r31,%r4

mr %r3,%r27 # CurrentShopItemIndex
# r4 already contains ShopItemsArray
mr %r5,%r29 # OffsetToCurrentItem
bl randomizeStandardShopItems

mr %r4,%r31
lwz %r31,0x8(%sp)
addi %sp,%sp,0xC

# Restore overwritten assembly
addi %r3,%sp,20

BranchBackRandomizeStandardShops:
b 0