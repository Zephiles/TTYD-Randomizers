.global StartArtAttackHitboxes
.global BranchArtAttackHitboxes

#ifdef TTYD_US
  battle_address_offset = 0x1C70
  unk_offset_1 = 0x0844
  unk_offset_2 = 0x0870
  unk_offset_3 = 0x0874
  unk_offset_4 = 0x0868
  unk_offset_5 = 0x0854
#elif defined TTYD_JP
  battle_address_offset = 0x1710
  unk_offset_1 = 0x03A4
  unk_offset_2 = 0x03D0
  unk_offset_3 = 0x03D4
  unk_offset_4 = 0x03C8
  unk_offset_5 = 0x03B4
#elif defined TTYD_EU
  battle_address_offset = 0x1D50
  unk_offset_1 = 0x08CC
  unk_offset_2 = 0x08F8
  unk_offset_3 = 0x08FC
  unk_offset_4 = 0x08F0
  unk_offset_5 = 0x08DC
#endif

StartArtAttackHitboxes:
bl checkIfArtAttackHitboxesEnabled

# Only run if the returned bool is true
cmpwi %r3,0 # False
beq+ Exit

stfd %f31, 0x0090 (%sp)
psq_st %f31, 152(%r1), 0, 0
stfd %f30, 0x0080 (%sp)
psq_st %f30, 136(%r1), 0, 0
stfd %f29, 0x0070 (%sp)
psq_st %f29, 120(%r1), 0, 0
stfd %f28, 0x0060 (%sp)
psq_st %f28, 104(%r1), 0, 0

lwz %r0, unk_offset_1 (%rtoc)
addi %r4, %sp, 8
lwz %r29, battle_address_offset (%r13)
li %r3, 4
stw %r0, 0x0008 (%sp)
bl GXSetChanMatColor
addi %r26, %sp, 56
addi %r27, %sp, 52
li %r28, 0
mr %r3, %r29
mr %r4, %r28
bl BattleGetUnitPtr
mr. %r30, %r3
beq- 0x324
bl BtlUnit_GetBelong
extsb %r0, %r3
cmpwi %r0, 1
bne- 0x314
mr %r3, %r30
li %r4, 27
bl BtlUnit_CheckStatus
cmpwi %r3, 0
bne- 0x300
mr %r3, %r30
bl BtlUnit_GetBodyPartsId
mr %r4, %r3
mr %r3, %r30
bl BtlUnit_GetPartsPtr
lwz %r0, 0x01AC (%r3)
rlwinm. %r0, %r0, 0, 15, 15
bne- 0x2E0
lwz %r0, 0x0104 (%r30)
rlwinm. %r0, %r0, 0, 30, 30
beq- 0x0C
lfs %f28, unk_offset_2 (%rtoc)
b 0x08
lfs %f28, unk_offset_3 (%rtoc)
lfs %f1, 0x00E4 (%r30)
mr %r3, %r30
lfs %f2, 0x0114 (%r30)
mr %r5, %r27
lfs %f0, 0x00E8 (%r30)
mr %r6, %r26
fmuls %f30,%f1,%f2
addi %r4, %sp, 48
fmuls %f29,%f0,%f2
bl BtlUnit_GetPos
lfs %f2, 0x00D8 (%r30)
addi %r3, %sp, 36
lfs %f4, 0x0114 (%r30)
mr %r4, %r3
lfs %f0, 0x0030 (%sp)
lfs %f1, unk_offset_4 (%rtoc)
fmadds %f3,%f2,%f4,%f0
lfs %f2, 0x0034 (%sp)
fmuls %f31,%f1,%f30
lfs %f0, 0x0038 (%sp)
fmuls %f30,%f1,%f29
stfs %f3, 0x0030 (%sp)
fsubs %f1,%f3,%f31
lfs %f3, 0x00DC (%r30)
fmuls %f3,%f3,%f4
stfs %f1, 0x0024 (%sp)
stfs %f0, 0x002C (%sp)
fmadds %f1,%f28,%f3,%f2
fadds %f0,%f1,%f30
stfs %f1, 0x0034 (%sp)
stfs %f0, 0x0028 (%sp)
bl btlGetScreenPoint
lfs %f0, 0x0030 (%sp)
addi %r3, %sp, 24
lfs %f1, 0x0034 (%sp)
mr %r4, %r3
fadds %f2,%f0,%f31
lfs %f0, 0x0038 (%sp)
fadds %f1,%f1,%f30
stfs %f0, 0x0020 (%sp)
stfs %f2, 0x0018 (%sp)
stfs %f1, 0x001C (%sp)
bl btlGetScreenPoint
li %r3, 168
li %r4, 0
li %r5, 2
bl GXBegin
lfs %f0, 0x0024 (%sp)
lis %r5, 0xCC01
addi %r3, %sp, 36
stfs %f0, -0x8000 (%r5)
mr %r4, %r3
lfs %f0, 0x0028 (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, unk_offset_5 (%rtoc)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x0018 (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x001C (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, unk_offset_5 (%rtoc)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x0030 (%sp)
lfs %f1, 0x0034 (%sp)
fadds %f2,%f0,%f31
lfs %f0, 0x0038 (%sp)
fadds %f1,%f1,%f30
stfs %f0, 0x002C (%sp)
stfs %f2, 0x0024 (%sp)
stfs %f1, 0x0028 (%sp)
bl btlGetScreenPoint
lfs %f0, 0x0030 (%sp)
addi %r3, %sp, 24
lfs %f1, 0x0034 (%sp)
mr %r4, %r3
fadds %f2,%f0,%f31
lfs %f0, 0x0038 (%sp)
fsubs %f1,%f1,%f30
stfs %f0, 0x0020 (%sp)
stfs %f2, 0x0018 (%sp)
stfs %f1, 0x001C (%sp)
bl btlGetScreenPoint
li %r3, 168
li %r4, 0
li %r5, 2
bl GXBegin
lfs %f0, 0x0024 (%sp)
lis %r5, 0xCC01
addi %r3, %sp, 36
stfs %f0, -0x8000 (%r5)
mr %r4, %r3
lfs %f0, 0x0028 (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, unk_offset_5 (%rtoc)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x0018 (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x001C (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, unk_offset_5 (%rtoc)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x0030 (%sp)
lfs %f1, 0x0034 (%sp)
fadds %f2,%f0,%f31
lfs %f0, 0x0038 (%sp)
fsubs %f1,%f1,%f30
stfs %f0, 0x002C (%sp)
stfs %f2, 0x0024 (%sp)
stfs %f1, 0x0028 (%sp)
bl btlGetScreenPoint
lfs %f0, 0x0030 (%sp)
addi %r3, %sp, 24
lfs %f1, 0x0034 (%sp)
mr %r4, %r3
fsubs %f2,%f0,%f31
lfs %f0, 0x0038 (%sp)
fsubs %f1,%f1,%f30
stfs %f0, 0x0020 (%sp)
stfs %f2, 0x0018 (%sp)
stfs %f1, 0x001C (%sp)
bl btlGetScreenPoint
li %r3, 168
li %r4, 0
li %r5, 2
bl GXBegin
lfs %f0, 0x0024 (%sp)
lis %r5, 0xCC01
addi %r3, %sp, 36
stfs %f0, -0x8000 (%r5)
mr %r4, %r3
lfs %f0, 0x0028 (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, unk_offset_5 (%rtoc)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x0018 (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x001C (%sp)
stfs %f0, -0x8000 (%r5)
lfs %f0, unk_offset_5 (%rtoc)
stfs %f0, -0x8000 (%r5)
lfs %f0, 0x0030 (%sp)
lfs %f1, 0x0034 (%sp)
fsubs %f2,%f0,%f31
lfs %f0, 0x0038 (%sp)
fsubs %f1,%f1,%f30
stfs %f0, 0x002C (%sp)
stfs %f2, 0x0024 (%sp)
stfs %f1, 0x0028 (%sp)
bl btlGetScreenPoint
lfs %f0, 0x0030 (%sp)
addi %r3, %sp, 24
lfs %f1, 0x0034 (%sp)
mr %r4, %r3
fsubs %f2,%f0,%f31
lfs %f0, 0x0038 (%sp)
fadds %f1,%f1,%f30
stfs %f0, 0x0020 (%sp)
stfs %f2, 0x0018 (%sp)
stfs %f1, 0x001C (%sp)
bl btlGetScreenPoint
li %r3, 168
li %r4, 0
li %r5, 2
bl GXBegin
lfs %f0, 0x0024 (%sp)
lis %r3, 0xCC01
stfs %f0, -0x8000 (%r3)
lfs %f0, 0x0028 (%sp)
stfs %f0, -0x8000 (%r3)
lfs %f0, unk_offset_5 (%rtoc)
stfs %f0, -0x8000 (%r3)
lfs %f0, 0x0018 (%sp)
stfs %f0, -0x8000 (%r3)
lfs %f0, 0x001C (%sp)
stfs %f0, -0x8000 (%r3)
lfs %f0, unk_offset_5 (%rtoc)
stfs %f0, -0x8000 (%r3)
addi %r28, %r28, 1
cmpwi %r28, 64
blt+ -0x33C
psq_l %f31, 152(%r1), 0, 0
lfd %f31, 0x0090 (%sp)
psq_l %f30, 136(%r1), 0, 0
lfd %f30, 0x0080 (%sp)
psq_l %f29, 120(%r1), 0, 0
lfd %f29, 0x0070 (%sp)
psq_l %f28, 104(%r1), 0, 0
lfd %f28, 0x0060 (%sp)

Exit:
# Restore overwritten assembly
lmw %r26, 0x0048 (%sp)

BranchArtAttackHitboxes:
b 0