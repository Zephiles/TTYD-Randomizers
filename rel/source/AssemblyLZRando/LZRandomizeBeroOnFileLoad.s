.global StartRandomizeBeroOnFileLoad
.global BranchBackRandomizeBeroOnFileLoad

StartRandomizeBeroOnFileLoad:
# r3 already contains swByteGetCheck
# r4 already contains GSWAddress
mr %r5,%r0 # NewValue
bl randomizeBeroOnFileLoad

BranchBackRandomizeBeroOnFileLoad:
b 0