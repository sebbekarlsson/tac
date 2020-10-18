.section .text
.globl _start
_start:
# pushl 0(%esp)
pushl 4(%esp)
call main
addl $4, %esp
movl %eax, %ebx
movl $1, %eax
int $0x80

.globl main
main:
pushl %ebp
movl %esp, %ebp
movl $4, %eax
movl $1, %ebx
# access
pushl 8(%esp)

movl (%esp), %ecx
# addl $0, %esp
movl $8, %edx
int $0x80
movl 8(%esp), %eax
movl %ebp, %esp
popl %ebp

ret
