# division
popl %eax
popl %ecx
div %ecx
pushl %eax
movb (%esp), %cl

