.global StartFillRunAwayMeter
.global BranchBackFillRunAwayMeter

StartFillRunAwayMeter:
mr %r3,%r30 # unkPtr
# f1 already contains CurrentRunMeterValue
bl checkIfRunAwayMeterFilled

BranchBackFillRunAwayMeter:
b 0