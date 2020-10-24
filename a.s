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
pushl $0x0a
pushl $0x021646c72
pushl $0x06f77206f
pushl $0x06c6c6548
pushl %esp
pushl $16
call print
pushl $0

jmp return_statement
print:
 pushl %ebp
 movl %esp, %ebp
 movl 12(%esp), %ecx
 movl 8(%esp), %edx
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
