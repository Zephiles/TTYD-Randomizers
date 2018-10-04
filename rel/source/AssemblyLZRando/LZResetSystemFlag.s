.global StartResetSystemFlag
.global BranchBackResetSystemFlag

StartResetSystemFlag:
# r3 already contains GSWAddress
bl resetSystemFlag
mr %r0,%r3

BranchBackResetSystemFlag:
b 0