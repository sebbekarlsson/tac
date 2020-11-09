.section .text
.globl _start
_start:
movl %esp, %ebp
call main
movl %eax, %ebx
movl $1, %eax
int $0x80

subl $8, %esp
.globl hello
hello:
pushl %ebp
movl %esp, %ebp
subl $32, %esp
subl $28, %esp
# variable (x)
pushl 8(%ebp)
call print
addl $0, %esp
pushl %eax
# variable (y)
pushl 12(%ebp)
call print
addl $0, %esp
pushl %eax
# variable (z)
pushl 16(%ebp)
call print
addl $0, %esp
pushl %eax
# integer
pushl $0
movb (%esp), %cl
movl $0, -28(%ebp)

jmp return_statement
movl %esp, -8(%ebp)
.globl main
main:
pushl %ebp
movl %esp, %ebp
subl $56, %esp
subl $52, %esp
# the first string

subl $24, %esp
movl $0x0, 20(%esp)
movl $0x0a, 16(%esp)
movl $0x0676e6972, 12(%esp)
movl $0x074732074, 8(%esp)
movl $0x073726966, 4(%esp)
movl $0x020656874, 0(%esp)
movl %esp, -32(%ebp)
movl %esp, -20(%ebp)
# the second string

subl $24, %esp
movl $0x0, 20(%esp)
movl $0x0a67, 16(%esp)
movl $0x06e697274, 12(%esp)
movl $0x07320646e, 8(%esp)
movl $0x06f636573, 4(%esp)
movl $0x020656874, 0(%esp)
movl %esp, -40(%ebp)
movl %esp, -28(%ebp)
# the third string

subl $24, %esp
movl $0x0, 20(%esp)
movl $0x0a, 16(%esp)
movl $0x0676e6972, 12(%esp)
movl $0x074732064, 8(%esp)
movl $0x072696874, 4(%esp)
movl $0x020656874, 0(%esp)
movl %esp, -48(%ebp)
movl %esp, -36(%ebp)
# variable (foo)
pushl -20(%ebp)
call print
addl $0, %esp
pushl %eax
# variable (bar)
pushl -28(%ebp)
call print
addl $0, %esp
pushl %eax
subl $4, %esp
# hey

subl $8, %esp
movl $0x0, 4(%esp)
movl $0x0a796568, 0(%esp)
movl %esp, -48(%ebp)
pushl -48(%ebp)
call print
addl $0, %esp
pushl %eax
# variable (far)
pushl -36(%ebp)
call print
addl $0, %esp
pushl %eax
# integer
pushl $0
movb (%esp), %cl
movl $0, -52(%ebp)

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

.type itos, @function
itos:
 pushl %ebp
 movl %esp, %ebp
  
 movl 8(%esp), %eax           # number
 movl $12, %edi
 leal (%esp, %edi, 1), %ebx   # buffer
 movl $0, %edi                # counter

 pushl $0x0
 jmp itos_loop 


itos_loop:
  movl $0, %edx

  movl $10, %ecx
  div %ecx
  
  addl $48, %edx
  pushl %edx

  movb (%esp), %cl
  movb %cl, (%ebx, %edi, 1)

  test %eax, %eax
  je itos_end

  dec %edi

  jmp itos_loop

itos_end:
  leal (%ebx, %edi, 1), %eax
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
