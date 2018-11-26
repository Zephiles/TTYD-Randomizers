.global StartFixShineBlockSystemLevel
.global BranchBackFixShineBlockSystemLevel

StartFixShineBlockSystemLevel:
stwu %sp,-0x10(%sp)
stmw %r30,0x8(%sp)
mr %r31,%r3
mr %r30,%r4

mr %r3,%r0 # _systemLevel
mr %r4,%r5 # _systemLevelAddress
bl fixShineBlockSystemLevel

mr %r4,%r30
mr %r3,%r31
lmw %r30,0x8(%sp)
addi %sp,%sp,0x10

BranchBackFixShineBlockSystemLevel:
b 0