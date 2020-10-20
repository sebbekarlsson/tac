.section .text
.globl _start
_start:
movl %esp, %ebp
call main
movl %eax, %ebx
movl $1, %eax
int $0x80

.globl main
main:
pushl %ebp
movl %esp, %ebp
pushl $1

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
