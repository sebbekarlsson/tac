# addition
popl %eax
addl (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl

