#ifndef __BIFLE_H_
#define __BFILE_H_
#define N 4096

#include <unistd.h>

struct bfile;
typedef struct bfile bfile;

bfile * bOpen (const char * path, char mode);
int bClose(bfile * bf);
ssize_t bWrite(void * p, int size, int nb_element, bfile * bf);
ssize_t bRead(void * p, int size, int nb_element, bfile * bf);
ssize_t bFlush(bfile * bf);
ssize_t bFill(bfile * bf);
#endif
