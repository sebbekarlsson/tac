.section .text
.globl _start
_start:
movl %esp, %ebp
call main
movl %eax, %ebx
movl $1, %eax
int $0x80

# compound (0x5558b707eb70)
.globl main
main:
pushl %ebp
movl %esp, %ebp
subl $52, %esp
subl $56, %esp
# compound (0x5558b707ee90)
# integer
pushl $5
movb (%esp), %cl
movl $5, -24(%ebp)
# integer
pushl $10
movb (%esp), %cl
movl $10, -20(%ebp)
# addition
popl %eax
addl (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl
# integer
pushl $10
movb (%esp), %cl
movl $10, -16(%ebp)
# addition
popl %eax
addl (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl
# assign (x)
movl $-28, %edi
movb %cl, (%ebp, %edi, 1)
# variable (x)
pushl -28(%ebp)
#  
subl $8, %esp
movl $0x0, 4(%esp)
movl $0x020, 0(%esp)
movl %esp, -40(%ebp)
# call arg
pushl -28(%ebp)
# call arg
pushl -40(%ebp)
call itos
addl $0, %esp
pushl %eax
movl %eax, -40(%ebp)
subl $8, %esp
# variable (value)
pushl -40(%ebp)
# call arg
pushl -40(%ebp)
call print
addl $0, %esp
pushl %eax
# integer
pushl $0
movb (%esp), %cl
movl $0, -48(%ebp)

jmp return_statement
movl %esp, -0(%ebp)
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
  
 movl 12(%esp), %eax           # number
 movl $8, %edi
 leal (%esp, %edi, 1), %ebx   # buffer
 movl $0, %edi                # counter
 movl $0, %esi

 pushl $0x0
 jmp itos_loop 


itos_loop:
  movl $0, %edx

  movl $10, %ecx
  div %ecx
  
  addl $48, %edx
  pushl %edx
  
  inc %edi

  test %eax, %eax
  je itos_buffer_loop

  jmp itos_loop

itos_buffer_loop: 
  popl %ecx
  movb %cl, (%ebx, %esi, 1)

  test %edi, %edi
  je itos_end

  inc %esi
  dec %edi

  jmp itos_buffer_loop

itos_end:
  movl %ebx, %eax
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

