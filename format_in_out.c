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


int fbWrite(bfile * bf, char * format, ...)
{
    char * str, *s, c;
    int x;
    
    va_list arg; 
    va_start(arg, format);

    //On parcourt la chaîne a écrire
    
    for(str=format; *str!='\0'; str++)
    {
        printf("%c", *str);
        //on parcourt jusqu'au prochain %
        while(*str != '%')
        {
            c = *str;
            bWrite(&c, 1, 1, bf);
            str++;
        }

        c = *str;
        switch(c)
        {
        case 'd':
            x = va_arg(arg,int);
            s = convert_int_to_str(x);
            bWrite(s, sizeof(s), 1, bf);
            break;
        case 'c':
            c = va_arg(arg,int);
            bWrite(&c, 1, 1, bf);
            break;
        case 's':
            s = va_arg(arg,char*);
            bWrite(s, sizeof(s), 1, bf);
            break;
        default:
            return -1;
        }
    }
    va_end(arg);
    return 1;
}
/*
int fbRead(bfile * bf,  char * format, ...)
{
    
}
*/
