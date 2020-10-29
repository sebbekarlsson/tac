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
pushl 32(%esp)
call print
addl $4, %esp
pushl 28(%esp)
call print
addl $4, %esp
pushl 24(%esp)
call print
addl $4, %esp
pushl 20(%esp)
call print
addl $4, %esp
pushl 16(%esp)
call print
addl $4, %esp
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
subl $4, %esp
subl $4, %esp
subl $4, %esp
subl $4, %esp
subl $4, %esp
subl $4, %esp
subl $4, %esp
subl $48, %esp
movl $0x0, 44(%esp)
movl $0x0a73, 40(%esp)
movl $0x0656e6f20, 36(%esp)
movl $0x072656874, 32(%esp)
movl $0x06f206568, 28(%esp)
movl $0x074206e61, 24(%esp)
movl $0x068742072, 20(%esp)
movl $0x065676e6f, 16(%esp)
movl $0x06c207469, 12(%esp)
movl $0x062206120, 8(%esp)
movl $0x073692074, 4(%esp)
movl $0x073726966, 0(%esp)
movl %esp, -4(%ebp)
# end of first is a bit longer than the other ones

subl $12, %esp
movl $0x0, 8(%esp)
movl $0x0a646e, 4(%esp)
movl $0x06f636573, 0(%esp)
movl %esp, -8(%ebp)
# end of second

subl $12, %esp
movl $0x0, 8(%esp)
movl $0x0a64, 4(%esp)
movl $0x072696874, 0(%esp)
movl %esp, -12(%ebp)
# end of third

subl $60, %esp
movl $0x0, 56(%esp)
movl $0x0a6b, 52(%esp)
movl $0x0726f7720, 48(%esp)
movl $0x06c6c6977, 44(%esp)
movl $0x020746920, 40(%esp)
movl $0x0796c6c75, 36(%esp)
movl $0x06665706f, 32(%esp)
movl $0x068207475, 28(%esp)
movl $0x062202c6f, 24(%esp)
movl $0x06f742067, 20(%esp)
movl $0x06e6f6c20, 16(%esp)
movl $0x074696220, 12(%esp)
movl $0x061206d61, 8(%esp)
movl $0x02049206f, 4(%esp)
movl $0x06c6c6568, 0(%esp)
movl %esp, -16(%ebp)
# end of hello I am a bit long too, but hopefully it will work

subl $12, %esp
movl $0x0, 8(%esp)
movl $0x0a, 4(%esp)
movl $0x065766966, 0(%esp)
movl %esp, -20(%ebp)
# end of five

subl $8, %esp
movl $0x0, 4(%esp)
movl $0x0a786973, 0(%esp)
movl %esp, -24(%ebp)
# end of six

subl $12, %esp
movl $0x0, 8(%esp)
movl $0x0a6e, 4(%esp)
movl $0x065766573, 0(%esp)
movl %esp, -28(%ebp)
# end of seven

pushl -4(%ebp)
pushl -8(%ebp)
pushl -12(%ebp)
pushl -16(%ebp)
pushl -20(%ebp)
pushl -24(%ebp)
pushl -28(%ebp)
call hello
addl $28, %esp
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
