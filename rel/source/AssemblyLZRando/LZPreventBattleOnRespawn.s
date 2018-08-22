.global StartPreventBattleOnRespawn
.global BranchBackPreventBattleOnRespawn

StartPreventBattleOnRespawn:
mr %r3,%r24 # fbatPointer
bl preventBattleOnRespawn

BranchBackPreventBattleOnRespawn:
b 0