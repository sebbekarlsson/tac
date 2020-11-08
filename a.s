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
subl $16, %esp
pushl $10
movb (%esp), %cl
# assign (x)
movl $-16, %edi
movb %cl, (%ebp, %edi, 1)
pushl $2
movb (%esp), %cl
# variable (x)
pushl -16(%ebp)
# division
popl %eax
popl %ecx
div %ecx
addl $4, %esp
pushl %eax
movb (%esp), %cl
call printi
addl $0, %esp
pushl $0
movb (%esp), %cl

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
  addl $4, %edi
  jmp strlenloop

strlenend:
  movl %edi, %eax
  movl %ebp, %esp
  popl %ebp
  ret

