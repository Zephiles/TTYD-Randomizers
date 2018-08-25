.global StartWarpAwaySQNo
.global BranchBackWarpAwaySQNo

StartWarpAwaySQNo:
# r3 already contains currentText
bl warpAwaySQNo

# Restore overwritten assembly
lwz %r4,0x8(%r31)

BranchBackWarpAwaySQNo:
b 0