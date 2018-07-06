.global StartAudienceItem
.global BranchBackAudienceItem

StartAudienceItem:
stwu %sp,-0x18(%sp)
stmw %r29,0x8(%sp)
mr %r31,%r3
mr %r30,%r4
mr %r29,%r6

GetRandomItem:
bl randomizeItem
cmpwi %r3,126 # GoldBar
blt- GetRandomItem
cmpwi %r3,331 # DebugBadge
beq- GetRandomItem

mr %r0,%r3

mr %r3,%r31
mr %r4,%r30
mr %r6,%r29
lmw %r29,0x8(%sp)
addi %sp,%sp,0x18

# Restore overwritten assembly
stw %r0,0x10(%r6)

BranchBackAudienceItem:
b 0