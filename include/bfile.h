#ifndef __BIFLE_H_
#define __BFILE_H_
#define N 4096

struct bfile;
typedef struct bfile bfile;

bfile * bOpen (const char * path, char mode);
int bClose(bfile * bf);
int bWrite(void * p, int size, int nb_element, bfile * bf);
int bRead(void * p, int size, int nb_element, bfile * bf);
int bFlush(bfile * bf);
int bFill(bfile * bf);
#endif
