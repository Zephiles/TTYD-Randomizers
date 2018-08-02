.global StartDisplayOverworldSPBars
.global BranchBackDisplayOverworldSPBars

StartDisplayOverworldSPBars:
# r3 already contains CurrentSpecialMoves
mr %r4,%r27 # CurrentSPBar
bl displayOverworldSPBars

# Check the returned bool
cmpwi %r3,0 # False

BranchBackDisplayOverworldSPBars:
b 0