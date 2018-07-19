.global StartEnablePaperTubeModes

StartEnablePaperTubeModes:
stwu %sp,-0xC(%sp)
stw %r31,0x8(%sp)
mflr %r31

# r3 already contains pouchCheckItem
bl enablePaperTubeModes

mtlr %r31
lwz %r31,0x8(%sp)
addi %sp,%sp,0xC

# Restore overwritten assembly
cmpwi %r3,0

# Branch back to calling function
blr