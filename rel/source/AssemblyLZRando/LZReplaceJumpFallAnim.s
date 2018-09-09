.global StartReplaceJumpFallAnim
.global BranchBackReplaceJumpFallAnim

StartReplaceJumpFallAnim:
stwu %sp,-0xC(%sp)
stw %r31,0x8(%sp)
mr %r31,%r0

mr %r3,%r25 # jumpFallString
bl replaceJumpFallAnim
mr %r4,%r3

mr %r0,%r31
lwz %r31,0x8(%sp)
addi %sp,%sp,0xC

BranchBackReplaceJumpFallAnim:
b 0