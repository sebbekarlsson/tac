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
pushl $0x0a216f
pushl $0x06c6c6548

movl %esp, %ecx
addl $8, %esp
movl $8, %edx

movl $4, %eax
movl $1, %ebx
int $0x80
pushl $1

jmp return_statement
.globl main
main:
pushl %ebp
movl %esp, %ebp
call hello
pushl $16

jmp return_statement
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
