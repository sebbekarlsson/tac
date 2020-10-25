.section .text
.globl _start
_start:
movl %esp, %ebp
call main
movl %eax, %ebx
movl $1, %eax
int $0x80

.globl hello
hello:
pushl %ebp
movl %esp, %ebp
pushl 12(%esp)
call print
addl $4, %esp
pushl 8(%esp)
call print
addl $4, %esp
pushl $1

jmp return_statement
.globl main
main:
pushl %ebp
movl %esp, %ebp
pushl $0x0
pushl $0x061
pushl %esp
pushl $0x0
pushl $0x062
pushl %esp
call hello
addl $8, %esp
pushl $0

jmp return_statement
print:
 pushl %ebp
 movl %esp, %ebp
 pushl 8(%esp)
 call strlen
 addl $4, %esp
 movl 8(%esp), %ecx
 movl %eax, %edx
 movl $4, %eax
 movl $1, %ebx
 movl %ebp, %esp
 popl %ebp
 int $0x80
 ret

return_statement:
 popl %eax
 movl %ebp, %esp
 popl %ebp

 ret

 .type strlen, @function
 strlen:
   pushl %ebp
   movl %esp, %ebp
   movl $0, %edi
   movl 8(%esp), %eax
   jmp strlenloop

 strlenloop:
   movb (%eax, %edi, 1), %cl
   cmpb $0, %cl
   je strlenend
   addl $1, %edi
   jmp strlenloop

 strlenend:
   movl %edi, %eax
   movl %ebp, %esp
   popl %ebp
   ret
