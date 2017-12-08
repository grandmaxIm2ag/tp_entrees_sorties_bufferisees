#include "include/format_in_out.h"
#include "include/bfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>

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
    char * str, *s,s1[N],s2[N], c, * c1;
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
                }
                seek=k;
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
                printf("||%d||\n", (seek<N && b2));
                for(k=seek; k<N && b2; k++ )
                {
                    if(!is_separator(s1[k]))
                    {
                        
                        s[j++] = s1[k];
                        printf("%c %c %s\n", s1[k], s[j-1], s);
                    }
                    else
                    {
                        b2=0;
                    }
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

