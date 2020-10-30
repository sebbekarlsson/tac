.section .text
.globl _start
_start:
movl %esp, %ebp
call main
movl %eax, %ebx
movl $1, %eax
int $0x80

subl $24, %esp
.globl hello
hello:
pushl %ebp
movl %esp, %ebp
pushl 8(%ebp)
call print
addl $4, %esp
pushl $0

jmp return_statement
movl %esp, -8(%ebp)
.globl main
main:
pushl %ebp
movl %esp, %ebp
subl $24, %esp
subl $72, %esp
movl $0x0, 68(%esp)
movl $0x0a676e69, 64(%esp)
movl $0x072747320, 60(%esp)
movl $0x0676e6f6c, 56(%esp)
movl $0x020797265, 52(%esp)
movl $0x076206120, 48(%esp)
movl $0x073692073, 44(%esp)
movl $0x069687420, 40(%esp)
movl $0x0646e6120, 36(%esp)
movl $0x065676175, 32(%esp)
movl $0x0676e616c, 28(%esp)
movl $0x020636174, 24(%esp)
movl $0x020656874, 20(%esp)
movl $0x020736920, 16(%esp)
movl $0x073696874, 12(%esp)
movl $0x020646c72, 8(%esp)
movl $0x06f77206f, 4(%esp)
movl $0x06c6c6568, 0(%esp)
movl %esp, -8(%ebp)
subl $16, %esp
movl $0x0, 12(%esp)
movl $0x0a7a7978, 8(%esp)
movl $0x020736920, 4(%esp)
movl $0x073696874, 0(%esp)
movl %esp, -12(%ebp)
pushl -12(%ebp)
call hello
addl $4, %esp
pushl $0

jmp return_statement
movl %esp, -12(%ebp)
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
