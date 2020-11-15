# subtraction
popl %eax
subl (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl

