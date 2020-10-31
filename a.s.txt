.section .text
.globl _start
_start:
movl %esp, %ebp
call main
movl %eax, %ebx
movl $1, %eax
int $0x80

subl $4, %esp
.globl main
main:
pushl %ebp
movl %esp, %ebp
subl $8, %esp
subl $4, %esp
subl $20, %esp
movl $0x0, 16(%esp)
movl $0x0a, 12(%esp)
movl $0x021646c72, 8(%esp)
movl $0x06f57206f, 4(%esp)
movl $0x06c6c6548, 0(%esp)
movl %esp, -4(%ebp)
# end of Hello World!

pushl -4(%ebp)
call print
addl $0, %esp
pushl $0

jmp return_statement
movl %esp, -8(%ebp)
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
