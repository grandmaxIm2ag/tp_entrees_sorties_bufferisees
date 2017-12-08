#include "include/format_in_out.h"
#include "include/bfile.h"
#include <stdio.h> 
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
char * next_str(bfile *bf, int seek);
{

    static char str[N];
    static int n = bRead(str, 1, N, bf);
    static int i =0, nb_read=1;
    int count = 0;
    char * res = (char *)malloc(sizeof(char)*N);
    do
    {
        nb_read++;
        for(i=i, i<n, i++)
        {
            if(!b && !is_separator(str[i]))
            {
                res[count++] = str[i];
            }
            else if(b && !is_end(str[i]))
            {
                res[count ++] = str[i];
            }
            else
            {
                return res;
            }
        }

        if((n=bRead(str, 1, N, bf))<=0)
        {
            return res;
        }
        realloc(res, nb_read*N);
        i=0;
    }while(1);
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
    char * str, s, c;
    int x, count=0;
    
    va_list arg; 
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
            x = va_arg(arg,int*);
            s = next_var(0, bf);
            count ++;
            bWrite(s, sizeof(s), 1, bf);
            break;
        case 'c':
            c = va_arg(arg,char*);
            count ++;
            bWrite(&c, 1, 1, bf);
            break;
        case 's':
            s = va_arg(arg,char**);
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

