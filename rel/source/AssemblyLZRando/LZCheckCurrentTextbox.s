.global StartCheckCurrentTextbox
.global BranchBackCheckCurrentTextbox

StartCheckCurrentTextbox:
# r3 already contains currentText
bl checkCurrentTextbox

# Restore overwritten assembly
lwz %r4,0x8(%r31)

BranchBackCheckCurrentTextbox:
b 0