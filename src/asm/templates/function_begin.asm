# start of "%1$s"
.globl %1$s
%1$s:
pushl %%ebp
movl %%esp, %%ebp
subl $%2$d, %%esp

