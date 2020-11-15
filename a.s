.section .text
.globl _start
_start:
movl %esp, %ebp
call main
movl %eax, %ebx
movl $1, %eax
int $0x80
# compound (0x56056613aee0)
# start of "hello"
.globl hello
hello:
pushl %ebp
movl %esp, %ebp
subl $16, %esp

subl $20, %esp
# compound (0x56056613b0d0)
# hello!

subl $12, %esp
movl $0x0, 8(%esp)
movl $0x0a216f, 4(%esp)
movl $0x06c6c6568, 0(%esp)
movl %esp, -16(%ebp)
# call arg
pushl -16(%ebp)
call print
addl $0, %esp
pushl %eax
# integer
pushl $0
movb (%esp), %cl
movl $0, -12(%ebp)

jmp return_statement
# assign default
movl %esp, -0(%ebp)

# start of "main"
.globl main
main:
pushl %ebp
movl %esp, %ebp
subl $56, %esp

subl $60, %esp
# compound (0x56056613b610)
# compound (0x56056613b7a0)
# integer
pushl $3
movb (%esp), %cl
movl $3, -24(%ebp)
# integer
pushl $6
movb (%esp), %cl
movl $6, -20(%ebp)
# subtraction
popl %eax
subl (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl

# integer
pushl $16
movb (%esp), %cl
movl $16, -16(%ebp)
# subtraction
popl %eax
subl (%esp), %eax
addl $4, %esp
pushl %eax
movb (%esp), %cl

# assign (x)
popl  -28(%ebp)

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
# assign call
movl %eax, -40(%ebp)
subl $8, %esp

# variable (value)
pushl -40(%ebp)
# call arg
pushl -40(%ebp)
call print
addl $0, %esp
pushl %eax
# 

subl $8, %esp
movl $0x0, 4(%esp)
movl $0x0a, 0(%esp)
movl %esp, -52(%ebp)
# call arg
pushl -52(%ebp)
call print
addl $0, %esp
pushl %eax
call hello
addl $0, %esp
pushl %eax
# integer
pushl $0
movb (%esp), %cl
movl $0, -52(%ebp)

jmp return_statement
# assign default
movl %esp, -4(%ebp)

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

