#ifndef TAC_IO_H
#define TAC_IO_H

char* tac_read_file(const char* filename);
void tac_write_file(const char* filename, char* outbuffer);
char* sh(const char* cmd);

#endif
