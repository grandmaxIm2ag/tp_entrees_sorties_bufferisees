#include "include/bfile.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

struct bfile
{
    FILE * f;
    char mode;
    char * buffer;
    unsigned file_seek;
    unsigned buffer_seek; 
    size_t size_buffer;
};

bfile * bOpen(const char * path, char mode){

    if(mode != 'E' && mode != 'L')
    {
        return NULL;
    }

    FILE * f = fopen( path , ((mode == 'E') ? "w+" : "r") );

    if(f == NULL)
    {
        return NULL;
    }
    
    bfile * bf = (bfile *) malloc(sizeof(bfile));
    bf -> f = f;
    bf -> mode = mode;
    bf -> file_seek = 0;
    bf -> buffer_seek = 0;
    bf -> size_buffer = N;
    bf -> buffer = (char *) malloc(sizeof(char)*bf->size_buffer);

    if(mode == 'L')
    {
        bFill(bf);
    }
    
    return bf;
}

int bClose(bfile * bf)
{
    if(bf == NULL)
    {
        return -1;
    }

    if(bf -> mode == 'R')
    {
        bFlush(bf);
    }
    
    int ret = fclose(bf->f);

    free(bf -> buffer);
    free(bf);
    
    return ret;
}


int bWrite(void * p, int size, int nb_element, bfile * bf)
{
    if(bf == NULL || p == NULL)
    {
        return -1;
    }

    if(bf -> mode != 'E')
    {
        return -1;
    }

    char * pp = (char*) p;

    int ret = 0;

    int i;
    for( i = 0 ; i < nb_element*size; i++ )
    {
        if(pp[i] != '\0')
        {
            bf -> buffer [bf -> buffer_seek++] =  pp[i];
            if(bf -> buffer_seek >= bf -> size_buffer
               ||  pp[i] == '\n' )
                {
                    bFlush(bf);
                }
            ret ++;
        }
        
    }
    return ret;
}

int bRead(void * p, int size, int nb_element, bfile * bf)
{

    if(bf == NULL || p==NULL)
    {
        return -1;
    }

    if(bf -> mode != 'L')
    {
        return -1;
    }

    char * pp = (char *) p;

    int i, ret=0;
    for( i = 0 ; i < nb_element*size-1 ; i++ )
    {
        if(bf -> buffer [bf -> buffer_seek] != '\0')
        {
            pp[i] = bf -> buffer [bf -> buffer_seek++];
            if(bf -> buffer_seek >= bf -> size_buffer-1)
             {
                 bFill(bf);
             }
            ret ++;
        }
        else
        {
            break;
        }

    }
    pp[i] = '\0';
    return ret;
}

int bFlush(bfile * bf)
{
    int ret = fwrite(bf -> buffer, 1, bf -> buffer_seek, bf -> f);
    fflush(bf->f);
    bf -> buffer_seek = 0;
    return ret;
}

int bFill(bfile * bf)
{
    int ret =  fread(bf -> buffer, 1,bf -> size_buffer -1, bf ->f);
    bf-> buffer_seek= 0;
    return ret;
}
