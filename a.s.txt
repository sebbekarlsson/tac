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
subl $12, %esp
movl $0x0, 8(%esp)
movl $0x0a6f, 4(%esp)
movl $0x06c6c6568, 0(%esp)
movl %esp, -4(%ebp)
# end of hello

pushl -4(%ebp)
call print
addl $0, %esp
subl $4, %esp
subl $44, %esp
movl $0x0, 40(%esp)
movl $0x0a646c, 36(%esp)
movl $0x0726f7720, 32(%esp)
movl $0x06f6c6c65, 28(%esp)
movl $0x068202c67, 24(%esp)
movl $0x06e697274, 20(%esp)
movl $0x073206f6c, 16(%esp)
movl $0x06c656820, 12(%esp)
movl $0x0676e6f6c, 8(%esp)
movl $0x020726568, 4(%esp)
movl $0x0746f6e61, 0(%esp)
movl %esp, -4(%ebp)
# end of another long hello string, hello world

pushl -4(%ebp)
call print
addl $0, %esp
pushl $1256434
call printi
addl $0, %esp
subl $4, %esp
subl $8, %esp
movl $0x0, 4(%esp)
movl $0x0a, 0(%esp)
movl %esp, -4(%ebp)
# end of 

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

.type printi, @function
printi:
 pushl %ebp
 movl %esp, %ebp

 movl 8(%esp), %eax

 pushl $0x0
 jmp printi_loop


printi_loop:
  movl $0, %edx

  movl $10, %ecx
  div %ecx

  addl $48, %edx
  pushl %edx

  test %eax, %eax
  je printi_end

  jmp printi_loop

printi_end:
  pushl %esp
  call strlen
  addl $4, %esp
  movl %esp, %ecx
  movl %eax, %edx
  movl $4, %eax
  movl $1, %ebx
  int $0x80

  movl %ebp, %esp
  popl %ebp
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
   addl $4, %edi
   jmp strlenloop

 strlenend:
   movl %edi, %eax
   movl %ebp, %esp
   popl %ebp
   ret
