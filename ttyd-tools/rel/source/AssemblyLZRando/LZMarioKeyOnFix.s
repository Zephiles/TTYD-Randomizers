.global StartMarioKeyOnFix
.global BranchBackMarioKeyOnFix

StartMarioKeyOnFix:
stwu %sp,-0x10(%sp)
stmw %r30,0x8(%sp)
mr %r31,%r4
mflr %r30

# r3 already contains currentKeyValue
bl fixMarioKeyOn
mr %r0,%r3

mtlr %r30
mr %r4,%r31
lmw %r30,0x8(%sp)
addi %sp,%sp,0x10

BranchBackMarioKeyOnFix:
b 0