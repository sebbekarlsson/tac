# access
movl $%d, %%edi
leal (%%ebp, %%edi, 1), %%eax
pushl %d(%%eax)
movl %d(%%eax), %%esp
movl %%esp, %d(%%ebp)

