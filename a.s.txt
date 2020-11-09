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
subl $16, %esp
# variable (x)
pushl 8(%ebp)
call print
addl $0, %esp
# variable (y)
pushl 12(%ebp)
call print
addl $0, %esp
# variable (z)
pushl 16(%ebp)
call print
addl $0, %esp
pushl $0
movb (%esp), %cl

jmp return_statement
movl %esp, -8(%ebp)
.globl main
main:
pushl %ebp
movl %esp, %ebp
subl $24, %esp
subl $24, %esp
movl $0x0, 20(%esp)
movl $0x0a, 16(%esp)
movl $0x0676e6972, 12(%esp)
movl $0x074732074, 8(%esp)
movl $0x073726966, 4(%esp)
movl $0x020656874, 0(%esp)
movl %esp, -16(%ebp)
subl $24, %esp
movl $0x0, 20(%esp)
movl $0x0a67, 16(%esp)
movl $0x06e697274, 12(%esp)
movl $0x07320646e, 8(%esp)
movl $0x06f636573, 4(%esp)
movl $0x020656874, 0(%esp)
movl %esp, -20(%ebp)
subl $24, %esp
movl $0x0, 20(%esp)
movl $0x0a, 16(%esp)
movl $0x0676e6972, 12(%esp)
movl $0x074732064, 8(%esp)
movl $0x072696874, 4(%esp)
movl $0x020656874, 0(%esp)
movl %esp, -24(%ebp)
# variable (foo)
pushl -16(%ebp)
call print
addl $0, %esp
subl $4, %esp
subl $12, %esp
movl $0x0, 8(%esp)
movl $0x0a, 4(%esp)
movl $0x06f796f79, 0(%esp)
movl %esp, -4(%ebp)
# end of yoyo

pushl -4(%ebp)
call print
addl $0, %esp
# variable (bar)
pushl -20(%ebp)
call print
addl $0, %esp
# variable (far)
pushl -24(%ebp)
call print
addl $0, %esp
pushl $256
movb (%esp), %cl
call printi
addl $0, %esp
pushl $0
movb (%esp), %cl

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
  call print
  addl $4, %esp

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
  addl $1, %edi
  jmp strlenloop

strlenend:
  movl %edi, %eax
  movl %ebp, %esp
  popl %ebp
  ret
 
