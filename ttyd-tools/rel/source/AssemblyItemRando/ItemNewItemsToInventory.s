.global StartNewItemsToInventory
.global BranchBackNewItemsToInventory

StartNewItemsToInventory:
stwu %sp,-0xC(%sp)
stw %r30,0x8(%sp)
mr %r30,%r3

lwz %r4,0(%r31) # Item to receive
cmplwi %r4,159 # HotDog
beq- GetRandomItem
cmplwi %r4,120 # CrystalStar
bgt+ Exit
cmplwi %r4,114 # DiamondStar
blt+ Exit

lis %r30,CrystalStarNewItem@ha
lhz %r4,CrystalStarNewItem@l(%r30)
b Exit

GetRandomItem:
bl randomizeItem
cmpwi %r3,126 #GoldBar
blt- GetRandomItem
mr %r4,%r3

li %r3,1 # True
lis %r30,RandomizeGivenItem@ha
stb %r3,RandomizeGivenItem@l(%r30)

Exit:
mr %r3,%r30
lwz %r30,0x8(%sp)
addi %sp,%sp,0xC

BranchBackNewItemsToInventory:
b 0