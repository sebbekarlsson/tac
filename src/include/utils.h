#ifndef TAC_UTILS_H
#define TAC_UTILS_H
void assert_not_null(void* x);
char* str_to_hex(const char* instr);
char** str_to_hex_chunks(const char* instr, int* nr_chunks);
char* mkstr(const char* str);
char* str_format(char* instr);
char str_to_escape_seq(const char* instr);
#endif
