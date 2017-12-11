#include "include/format_in_out.h"
#include "include/bfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>

/**
 *La structure pour l'accès à un fichier f, ouvert en mode lecture ou en écriture se fassent
 *par le biais d'un buffer pour gèrerer l'allocation, libération, le remplissage et le vidage. 
 */
struct bfile
{
    FILE * f;
    char mode;
    char * buffer;
    unsigned file_seek;
    unsigned buffer_seek; 
    size_t size_buffer;
};
/**
 *Le but de la fonction est ouvrir le fichier avec le nom path uniquement avec le mode
 *d'ouverture en lecture ('L') ou en écriture 'E' et allouer la structure de type FICHIER
 *avec son tampon et son descripteur de fichier. La fonction renvoie NULL si le fichier
 *ne peut pas être ouvert au contraire un pointeursur un objet de type FICHIER. 
 */
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
            printf("%c",bf -> buffer [bf -> buffer_seek] );
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
/*
char * convert_int_to_str(int x)
{
    static char buff[11];
    char * res;
    res = &buff[10];
    *res = '\0';

    int n = x;

    do
    {
        char c = (n % 10) + 48;
        *(--res) = c;
        n /= 10;
    }while(n > 0);
    return res;
}

unsigned is_separator(char c)
{
    return (c=='\n' || c==' ' || c=='\t' || c=='\0');
}

int fbWrite(bfile * bf, char * format, ...)
{
    char * str, *s, c;
    
    int x, count=0;
    
    va_list arg; 
    va_start(arg, format);

    //On parcourt la chaîne a écrire
    
    for(str=format; *str!='\0'; str++)
    {
        //on parcourt jusqu'au prochain %
        while(*str != '%')
        {
            c = *str;
            count ++;
            bWrite(&c, 1, 1, bf);
            str++;
        }

        //On récupert le caractère suivant le %
        str++;
        c = *str;
        switch(c)
        {
        case 'd':
            x = va_arg(arg,int);
            s = convert_int_to_str(x);
            count ++;
            bWrite(s, sizeof(s), 1, bf);
            break;
        case 'c':
            c = va_arg(arg,int);
            count ++;
            bWrite(&c, 1, 1, bf);
            break;
        case 's':
            s = va_arg(arg,char*);
            count ++;
            bWrite(s, sizeof(s), 1, bf);
            break;
        default:
            return count;
        }
    }
    va_end(arg);
    
    return count;
}


int fbRead(bfile * bf,  char * format, ...)
{
    char * str, *s,s1[N],*s2, c, * c1;
    s2 = (char *)malloc(N);
    int *x, count=0, seek=0, k, j;
    unsigned b = 1, b2;
    va_list arg;
    bRead(s1, 1, N, bf);
    va_start(arg, format);

    //On parcourt la chaîne a écrire
    
    for(str=format; *str!='\0'; str++)
    {
        //on parcourt jusqu'au prochain %
        while(*str != '%')
        {
            str++;
        }
        
        //On récupert le caractère suivant le %
        str++;
        c = *str;
        switch(c)
        {
        case 'd':
            x = (int *)va_arg(arg,int*);
            k=0;
            b2=1;
            do
            {
                for(k=seek; k<N && b2; k++ )
                {
                    if(!is_separator(s1[k]))
                    {
                        s2[k] = s1[k];
                    }
                    else
                    {
                        b2=0;
                    }
                    if(j>=N)
                    {
                        s2=realloc(s2, sizeof(s2)+N);
                    }
                }
                seek=k;
                if((k=bRead(s1, 1, N, bf)))
                {
                    b = 0;
                }
            }while(b && b2);
            *x = atoi(s2);
            count ++;
            break;
        case 'c':
            c1 = (char *) va_arg(arg,char*);
            *c1 = s1[seek++];
            count ++;
            break;
        case 's':
            s = (char *)va_arg(arg,char*);
            b2=1;
            j=0;
            do
            {
                for(k=seek; k<N && b2; k++ )
                {
                    if(!is_separator(s1[k]))
                    {
                        
                        s[j++] = s1[k];
                    }
                    else
                    {
                        b2=0;
                    }
                    if(j>=N)
                    {
                        s=realloc(s, sizeof(s2)+N);
                    }
                }
                if(b2 && (k=bRead(s1, 1, N, bf)))
                {
                    b = 0;
                }
                seek=k;
            }while(b && b2);
            s[seek] = '\0';
            count ++;
            break;
        default:
            return count;
        }
    }
    va_end(arg);
    
    return count;
}

*/
