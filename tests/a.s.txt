.section .text
.globl _start
_start:
movl %esp, %ebp
call main
movl %eax, %ebx
movl $1, %eax
int $0x80
# compound (0x561935e0eda0)
# start of "main"
.globl main
main:
pushl %ebp
movl %esp, %ebp
subl $56, %esp

subl $60, %esp
# compound (0x561935e0f0c0)
# integer
pushl $1
movb (%esp), %cl
movl $1, -28(%ebp)
# compound (0x561935e0f230)
# integer
pushl $5
movb (%esp), %cl
movl $5, -24(%ebp)
# integer
pushl $5
movb (%esp), %cl
movl $5, -20(%ebp)
# multiplication
popl %eax
imull (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl

# integer
pushl $5
movb (%esp), %cl
movl $5, -16(%ebp)
# addition
popl %eax
addl (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl

# subtraction
popl %eax
subl (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl

# assign (res)
popl  -32(%ebp)

 # variable (res)
pushl -32(%ebp)
#  
subl $8, %esp
movl $0x0, 4(%esp)
movl $0x020, 0(%esp)
movl %esp, -44(%ebp)
# call arg
pushl -32(%ebp)
# call arg
pushl -44(%ebp)
call itos
addl $0, %esp
pushl %eax
# assign call
movl %eax, -44(%ebp)
subl $8, %esp

# variable (value)
pushl -44(%ebp)
# call arg
pushl -44(%ebp)
call print
addl $0, %esp
pushl %eax
# integer
pushl $0
movb (%esp), %cl
movl $0, -52(%ebp)

jmp return_statement
# assign default
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

