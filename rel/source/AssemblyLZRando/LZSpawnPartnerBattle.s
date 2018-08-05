.global StartSpawnPartnerBattle
.global BranchBackSpawnPartnerBattle

StartSpawnPartnerBattle:
# r3 already contains PartnerValue
bl spawnPartnerBattle

# Restore overwritten assembly
cmpwi %r3,0

BranchBackSpawnPartnerBattle:
b 0