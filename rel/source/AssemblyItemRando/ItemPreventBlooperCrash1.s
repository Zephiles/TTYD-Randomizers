.global StartPreventBlooperCrash1
.global BranchBackPreventBlooperCrash1

StartPreventBlooperCrash1:
mr %r3,%r31 # unkValue
# r4 already contains BattleUnitPointer
bl preventBlooperCrash1

BranchBackPreventBlooperCrash1:
b 0