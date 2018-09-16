.global StartPreventTextboxOptions
.global BranchBackPreventTextboxOptions

StartPreventTextboxOptions:
mr %r3,%r30 # currentText
mr %r4,%r29 # storeAddress
mr %r5,%r0 # selectedOption
bl preventTextboxOptions

BranchBackPreventTextboxOptions:
b 0