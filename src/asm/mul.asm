# multiplication
popl %eax
imull (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl

